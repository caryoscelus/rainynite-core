/*
 *  abstract_value.h - Node abstract Value
 *  Copyright (C) 2017 caryoscelus
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CORE__NODE__ABSTRACT_VALUE_H__ACCED1BE
#define __CORE__NODE__ABSTRACT_VALUE_H__ACCED1BE

#include <boost/any.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "notify.h"
#include "abstract_list.h"

namespace rainynite::core {

using Id = boost::uuids::uuid;

class AbstractValue : public AbstractNotify {
public:
    virtual bool is_const() const {
        return false;
    }
    virtual Type get_type() const = 0;
    virtual boost::any get_any(std::shared_ptr<Context> context) const = 0;
    virtual void set_any(boost::any const& /*value*/) {
        throw NodeAccessError("Cannot set");
    }
    virtual bool can_set_any(boost::any const& /*value*/) const {
        return false;
    }
    virtual boost::any any() const {
        throw NodeAccessError("No static value");
    }
    virtual void set_source(std::shared_ptr<AbstractValue> /*source*/) {
        throw NodeAccessError("Cannot set source node");
    }
    virtual bool can_set_source(std::shared_ptr<AbstractValue> /*source*/) const {
        return false;
    }
    /**
     * Call arbitrary function with all dynamic list member nodes in context.
     *
     * If this node is not a list, throws NodeAccessError
     */
    virtual void step_into_list(std::shared_ptr<Context> /*context*/, std::function<void(NodeInContext)> /*f*/) const {
        throw NodeAccessError("This node is not a list");
    }
    virtual std::vector<NodeInContext> get_list_links(std::shared_ptr<Context> ctx) const {
        std::vector<NodeInContext> result;
        step_into_list(ctx, [&result](auto e) {
            result.push_back(std::move(e));
        });
        return result;
    }
public:
    Id get_id() {
        return id;
    }
    void set_id(Id id_) {
        id = id_;
    }
    void new_id() {
        id = boost::uuids::random_generator()();
    }
private:
    Id id;
};

template <typename T>
constexpr bool is_vector = false;

template <typename U>
constexpr bool is_vector<std::vector<U>> = true;

template <typename T>
class BaseValue : public AbstractValue {
public:
    virtual T get(std::shared_ptr<Context> context) const {
        if constexpr (is_vector<T>) {
            using E = typename T::value_type;
            T result;
            step_into_list(
                context,
                [&result](NodeInContext nic) {
                    if (auto value = dynamic_cast<BaseValue<E>*>(nic.node.get())) {
                        result.push_back(value->get(nic.context));
                    }
                }
            );
            return result;
        } else {
            (void) context;
            throw NodeAccessError("Get not implemented in node");
        }
    }
    virtual void set(T /*value*/) {
        throw NodeAccessError("Cannot set");
    }
    virtual T& mod() {
        throw NodeAccessError("Cannot set");
    }
    virtual bool can_set() const {
        return false;
    }
public:
    Type get_type() const override {
        return typeid(T);
    }
    boost::any get_any(std::shared_ptr<Context> context) const override {
        return get(context);
    }
    void set_any(boost::any const& value_) override {
        set(boost::any_cast<T>(value_));
    }
    bool can_set_any(boost::any const& value_) const override {
        return can_set() && value_.type() == typeid(T);
    }
    void step_into_list(std::shared_ptr<Context> context, std::function<void(NodeInContext)> f) const override {
        if constexpr (is_vector<T>) {
            for (auto&& e : get_list_links(context))
                f(e);
        } else {
            AbstractValue::step_into_list(context, f);
        }
    }
public:
    static Type static_type() {
        return typeid(T);
    }
};

} // namespace rainynite::core

#endif
