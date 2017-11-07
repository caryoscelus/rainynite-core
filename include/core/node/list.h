/*  list.h - list
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

#ifndef CORE_NODE_LIST_H_12D92F7F_8F30_5447_9F54_AFB006578F3D
#define CORE_NODE_LIST_H_12D92F7F_8F30_5447_9F54_AFB006578F3D

#include <core/std/traits.h>
#include <core/nothing.h>
#include "abstract_list.h"
#include "abstract_value.h"
#include "make.h"

namespace rainynite::core {

template <typename T, typename P>
void push_value(P list, T const& value) {
    list->push_back(make_value<T>(value));
}

/**
 * Base template for typed and mixed lists
 */
template <class S, class P>
class ListValueBase : public AbstractListLinked, public P {
public:
    vector<AbstractReference> get_links() const override {
        if constexpr (is_same_v<S, AbstractValue>)
            return values;
        vector<AbstractReference> result;
        std::transform(
            values.begin(),
            values.end(),
            std::back_inserter(result),
            [](auto e) {
                return e;
            }
        );
        return result;
    }

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> context) const override {
        vector<NodeInContext> result;
        std::transform(
            values.begin(),
            values.end(),
            std::back_inserter(result),
            [context](auto&& e) {
                return NodeInContext(e, context);
            }
        );
        return result;
    }

public:
    AbstractReference get_link(size_t i) const override {
        return values.at(i);
    }
    size_t link_count() const override {
        return values.size();
    }
    void set_link(size_t i, AbstractReference value) override {
        if (auto node = dynamic_pointer_cast<S>(std::move(value))) {
            values.at(i) = node;
            signal_connections[i].disconnect();
            signal_connections[i] = node->subscribe([this]() {
                this->changed();
            });
            this->changed();
        }
    }
    void push_back(AbstractReference value) override {
        insert(link_count(), value);
    }
    void push_new() override {
        push_value<Nothing>(this, {});
    }
    void insert(size_t i, AbstractReference value) override {
        if (auto e = dynamic_pointer_cast<S>(std::move(value))) {
            values.insert(values.begin()+i, e);
            signal_connections.insert(
                signal_connections.begin()+i,
                e->subscribe([this]() {
                    this->changed();
                })
            );
            this->changed();
        } else {
            // TODO: throw?
        }
    }
    void remove(size_t index) override {
        values.erase(values.begin()+index);
        signal_connections.erase(signal_connections.begin()+index);
        this->changed();
    }
    void pop() override {
        remove(values.size()-1);
    }
    bool is_editable_list() const override {
        return true;
    }
protected:
    vector<shared_ptr<S>> values;
    vector<boost::signals2::connection> signal_connections;
};

/**
 * Typed (homogeneous) list
 */
template <typename T>
class ListValue : public ListValueBase<BaseValue<T>, BaseValue<vector<T>>> {
public:
    ListValue() = default;
public:
    vector<T> get(shared_ptr<Context> context) const override {
        // TODO: caching
        vector<T> result;
        std::transform(
            this->values.begin(),
            this->values.end(),
            std::back_inserter(result),
            [context](auto e) {
                return e->get(context);
            }
        );
        return result;
    }
    TypeConstraint get_link_type(size_t) const override {
        return {Type(typeid(T))};
    }
    void push_new() override {
        push_value<T>(this, {});
    }
};

/**
 * Untyped (heterogeneous) list value
 */
class UntypedListValue : public ListValueBase<AbstractValue, AbstractValue> {
public:
    Type get_type() const override {
        return typeid(Nothing);
    }
    any get_any(shared_ptr<Context> /*context*/) const noexcept override {
        return Nothing();
    }
    TypeConstraint get_link_type(size_t) const override {
        return {};
    }
public:
    static Type static_type() {
        return typeid(Nothing);
    }
};

} // namespace rainynite::core

#endif
