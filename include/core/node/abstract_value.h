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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <core/std/any.h>
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
    virtual any get_any(shared_ptr<Context> context) const noexcept = 0;
    virtual void set_any(any const& /*value*/) {
        throw NodeAccessError("Cannot set");
    }
    virtual bool can_set_any(any const& /*value*/) const {
        return false;
    }
    virtual any static_any() const {
        throw NodeAccessError("No static value");
    }
    virtual void set_source(shared_ptr<AbstractValue> /*source*/) {
        throw NodeAccessError("Cannot set source node");
    }
    virtual bool can_set_source(shared_ptr<AbstractValue> /*source*/) const {
        return false;
    }
    /**
     * Call arbitrary function with all dynamic list member nodes in context.
     *
     * If this node is not a list, throws NodeAccessError
     */
    virtual void step_into_list(shared_ptr<Context> /*context*/, std::function<void(NodeInContext)> /*f*/) const {
        throw NodeAccessError("This node is not a list");
    }
    virtual vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const {
        vector<NodeInContext> result;
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
constexpr bool is_vector<vector<U>> = true;

template <typename T>
class BaseValue : public AbstractValue {
public:
    /**
     * Get value of this node in given context
     *
     * Any exceptions that are thrown during node calculation should be
     * recorded in log (TODO; not yet implemented).
     */
    T value(shared_ptr<Context> context) const noexcept {
        try {
            return get(context);
        } catch (...) {
            // TODO: log exceptions
            return {};
        }
    }
public:
    /**
     * Get value of this node in given context - may throw
     *
     * TODO: make protected
     */
    virtual T get(shared_ptr<Context> context) const {
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
    any get_any(shared_ptr<Context> context) const noexcept override {
        return value(context);
    }
    void set_any(any const& value_) override {
        set(any_cast<T>(value_));
    }
    bool can_set_any(any const& value_) const override {
        return can_set() && value_.type() == typeid(T);
    }
    void step_into_list(shared_ptr<Context> context, std::function<void(NodeInContext)> f) const override {
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
