/*  abstract_value.h - Node abstract Value
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

#ifndef CORE_NODE_ABSTRACT_VALUE_H_D0A0875B_B140_5BFA_9B2E_C7C291245E53
#define CORE_NODE_ABSTRACT_VALUE_H_D0A0875B_B140_5BFA_9B2E_C7C291245E53

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <core/std/any.h>
#include <core/log/simple_exception_log.h>
#include <core/log/global.h>
#include "notify.h"
#include "has_id.h"
#include "abstract_list.h"
#include "cast.h"

namespace rainynite::core {

using NodeId = boost::uuids::uuid;
using NodeIdGenerator = boost::uuids::random_generator;

class Enabled {
public:
    explicit Enabled(bool value=true) :
        is_enabled(value)
    {}

    void set_enabled(bool value) {
        is_enabled = value;
    }
    bool enabled() const {
        return is_enabled;
    }
private:
    bool is_enabled;
};

class AbstractValue :
    public AbstractNotify,
    public HasId<NodeId, NodeIdGenerator>,
    public Enabled
{
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
     * Get list of NodeInContext links if this node is a list.
     *
     * This method exists to allow nodes performing operations on lists without
     * necessarily calculating each list element value. It returns list of
     * NodeInContext in order to support dynamic list nodes that change context
     * for their children.
     */
    vector<NodeInContext> list_links(shared_ptr<Context> ctx) const noexcept {
        if (!enabled())
            return {};
        try {
            return get_list_links(ctx);
        } catch (...) {
            // TODO: report
            return {};
        }
    }

protected:
    /// list_links virtual implementation.
    virtual vector<NodeInContext> get_list_links(shared_ptr<Context> /*ctx*/) const {
        throw NodeAccessError("This node is not a list");
    }
};

template <typename T>
constexpr bool is_vector = false;

template <typename U>
constexpr bool is_vector<vector<U>> = true;

template <typename T>
class BaseValue :
    public AbstractValue,
    public HasExceptionLogger
{
public:
    BaseValue() :
        HasExceptionLogger(make_shared<GlobalLog<SimpleExceptionLogger>>())
    {}
public:
    /**
     * Get value of this node in given context
     *
     * Any exceptions that are thrown during node calculation should be
     * recorded in log (TODO; not yet implemented).
     */
    T value(shared_ptr<Context> context) const noexcept {
        if (!enabled())
            return default_value(context);
        try {
            return get(context);
        } catch (std::exception const& ex) {
            log_exception_from_this(ex);
            return {};
        } catch (...) {
            log_exception_from_this(NodeAccessError("Unknown error in BaseValue::value()"));
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
            auto links = list_links(context);
            bool error = false;
            std::transform(
                links.begin(),
                links.end(),
                std::back_inserter(result),
                [&error](NodeInContext e) {
                    if (auto node = base_value_cast<E>(std::move(e.node)))
                        return node->value(e.context);
                    error = true;
                    return E{};
                }
            );
            if (error)
                throw NodeAccessError("Got wrong element type");
            return result;
        } else {
            (void) context;
            throw NodeAccessError("Get not implemented in node");
        }
    }

    /// Value to return when node is disabled
    virtual T default_value(shared_ptr<Context> /*ctx*/) const noexcept {
        return {};
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
public:
    static Type static_type() {
        return typeid(T);
    }
};

} // namespace rainynite::core

#endif
