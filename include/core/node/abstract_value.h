/*  abstract_value.h - Node abstract Value
 *  Copyright (C) 2017-2018 caryoscelus
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
#include <boost/uuid/uuid_io.hpp>

#include <core/std/any.h>
#include <core/log/simple_exception_log.h>
#include <core/log/global.h>
#include "notify.h"
#include "has_id.h"
#include "abstract_list.h"
#include "cast.h"
#include "node_in_context.h"

namespace rainynite::core {

template <typename UuidGen>
struct UuidStringGen {
    string operator()() const {
        return to_string(UuidGen()());
    }
};

using NodeId = string;
using NodeIdGenerator = UuidStringGen<boost::uuids::random_generator>;

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

/**
 * Base class for node system.
 *
 * All nodes must inherit this class.
 */
class AbstractValue :
    public AbstractNotify,
    public HasId<NodeId, NodeIdGenerator>,
    public HasExceptionLogger,
    public Enabled,
    public enable_shared_from_this<AbstractValue>
{
public:
    AbstractValue() :
        HasExceptionLogger(make_shared<GlobalLog<SimpleExceptionLogger>>())
    {}

    /// Returns whether this node accepts values to be set with set_any/set/etc
    virtual bool can_set() const {
        return false;
    }

    /**
     * Returns type of this node.
     *
     * Currently most nodes has only one type at all times. However, alternative
     * approach is being considered (see AnyProxy node).
     */
    virtual Type get_type() const = 0;

    /**
     * Returns value of this node in context.
     *
     * This is generic version, returning any value. As most nodes has only
     * single type and derive from BaseValue, they don't have to have custom
     * implementation of this function.
     */
    virtual any get_any(shared_ptr<Context> ctx) const noexcept = 0;

    /// Shortcut for any_cast<T>(get_any(..))
    template <typename T>
    T get_any_as(shared_ptr<Context> ctx) const {
        return any_cast<T>(get_any(ctx));
    }

    /// Set any value, if supported; throw error otherwise
    virtual void set_any(any const& /*value*/) {
        throw NodeAccessError("Cannot set");
    }

    /**
     * Returns whether this node can accept the value.
     *
     * For nodes derived from BaseValue, this shouldn't be overriden.
     *
     * See also: can_set
     */
    virtual bool can_set_any(any const& /*value*/) const {
        return false;
    }

    /**
     * Similar to set_any, but sets value with respect to context.
     *
     * Unlike set_any, can be implemented on dynamic nodes. Exact effect may
     * vary on node, but if this operation succeeds, `get_any(ctx) == value`
     * should be true.
     */
    virtual void set_any_at(any const& value, shared_ptr<Context> /*ctx*/) {
        set_any(value);
    }

    /// Returns whether this node supports set_any_at
    virtual bool can_set_any_at() const {
        return can_set();
    }

    /// Returns whether this node ignores context (i.e. holds static value)
    virtual bool is_static() const {
        return false;
    }

    /// If is_static(), returns static value, otherwise throws
    virtual any static_any() const {
        throw NodeAccessError("No static value");
    }

    /**
     * If supported, sets `source` node as a source for its value.
     *
     * This is usually the first property of filter-style nodes.
     */
    virtual void set_source(shared_ptr<AbstractValue> /*source*/) {
        throw NodeAccessError("Cannot set source node");
    }

    /// Returns whether `source` can be passed to set_source
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

    /**
     * Get size of represented list.
     *
     * Returns 0 by default.
     */
    virtual size_t list_links_count(shared_ptr<Context> /*ctx*/) const {
        return 0;
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

/**
 * Base template for nodes with static type.
 *
 * Currently all nodes are supposed to derive from this, however this can be
 * changed eventually, so if possible, do not rely on BaseValue casts and use
 * get_type() & any-based operations instead.
 */
template <typename T>
class BaseValue : public AbstractValue {
public:
    /**
     * Get value of this node in given context
     *
     * Any exceptions that are thrown during node calculation
     * are recorded in log.
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

    virtual void set(T /*value*/) {
        throw NodeAccessError("Cannot set");
    }
    virtual T& mod() {
        throw NodeAccessError("Cannot set");
    }

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

    static Type static_type() {
        return typeid(T);
    }

protected:
    /**
     * Get value of this node in given context - may throw
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
};

} // namespace rainynite::core

#endif
