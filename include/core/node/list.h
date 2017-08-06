/*
 *  list.h - list
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

#ifndef __CORE__NODE__LIST_H__5C3B88BA
#define __CORE__NODE__LIST_H__5C3B88BA

#include "abstract_list.h"
#include "abstract_value.h"

#include <core/nothing.h>

namespace core {

/**
 * Base template for typed and mixed lists
 */
template <class S, class P>
class ListValueBase : public AbstractListLinked, public P {
public:
    std::vector<AbstractReference> get_links() const override {
        if constexpr (std::is_same_v<S, AbstractValue>)
            return values;
        std::vector<AbstractReference> result;
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
    void step_into_list(Time time, std::function<void(AbstractReference,Time)> f) const override {
        for (auto child : values) {
            f(child, time);
        }
    }
    AbstractReference get_link(size_t i) const override {
        return values.at(i);
    }
    size_t link_count() const override {
        return values.size();
    }
    void set_link(size_t i, AbstractReference value) override {
        if (auto node = std::dynamic_pointer_cast<S>(std::move(value))) {
            values.at(i) = node;
            signal_connections[i].disconnect();
            signal_connections[i] = node->subscribe([this]() {
                this->changed();
            });
            this->changed();
        }
    }
    void push_back(AbstractReference value) override {
        if (auto e = std::dynamic_pointer_cast<S>(std::move(value))) {
            values.push_back(e);
            signal_connections.push_back(e->subscribe([this]() {
                this->changed();
            }));
            this->changed();
        } else {
            //throw
        }
    }
    void push_new() override {
        push_value<Nothing>({});
    }
    void remove(size_t index) override {
        values.erase(values.begin()+index);
        signal_connections.erase(signal_connections.begin()+index);
    }
    bool is_editable_list() const override {
        return true;
    }
protected:
    std::vector<std::shared_ptr<S>> values;
    std::vector<boost::signals2::connection> signal_connections;
};

/**
 * Typed (homogeneous) list
 */
template <typename T>
class ListValue : public ListValueBase<BaseValue<T>, BaseValue<std::vector<T>>> {
public:
    ListValue() = default;
public:
    std::vector<T> get(Time t) const override {
        // TODO: caching
        std::vector<T> result;
        std::transform(
            this->values.begin(),
            this->values.end(),
            std::back_inserter(result),
            [t](auto e) {
                return e->get(t);
            }
        );
        return result;
    }
    boost::optional<Type> get_link_type(size_t) const override {
        return boost::make_optional(Type(typeid(T)));
    }
    void push_new() override {
        this->template push_value<T>({});
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
    boost::any get_any(Time) const {
        return Nothing();
    }
    boost::optional<std::type_index> get_link_type(size_t) const override {
        return boost::none;
    }
public:
    static Type static_type() {
        return typeid(Nothing);
    }
};

} // namespace core

#endif
