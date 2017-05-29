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

namespace core {

template <typename T>
class ListValue : public BaseValue<std::vector<T>>, public AbstractListLinked {
public:
    ListValue() = default;
public:
    virtual std::vector<T> get(Time t) const override {
        // TODO: caching
        std::vector<T> result;
        std::transform(
            values.begin(),
            values.end(),
            std::back_inserter(result),
            [t](auto e) {
                return e->get(t);
            }
        );
        return result;
    }
    virtual std::vector<AbstractReference> get_links() const override {
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
    virtual AbstractReference get_link(size_t i) const override {
        return values.at(i);
    }
    virtual boost::optional<Type> get_link_type(size_t) const override {
        return boost::make_optional(Type(typeid(T)));
    }
    virtual void set_link(size_t i, AbstractReference value) override {
        if (auto node = std::dynamic_pointer_cast<BaseValue<T>>(std::move(value)))
            values.at(i) = node;
    }
    virtual size_t link_count() const override {
        return values.size();
    }
    virtual void push_back(AbstractReference value) override {
        if (auto e = std::dynamic_pointer_cast<BaseValue<T>>(value)) {
            values.push_back(e);
        } else {
            //throw
        }
    }
    virtual void push_new() override {
        push_value<T>({});
    }
    virtual void remove(size_t index) override {
        values.erase(values.begin()+index);
    }
    virtual bool is_editable_list() const override {
        return true;
    }
private:
    std::vector<BaseReference<T>> values;
};

} // namespace core

#endif
