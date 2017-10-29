/*  link_storage.h - link storage
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

#ifndef CORE_NODE_LINK_STORAGE_H_56A4E74C_E276_5ADA_A9E9_06154E870892
#define CORE_NODE_LINK_STORAGE_H_56A4E74C_E276_5ADA_A9E9_06154E870892

#include <core/std/array.h>
#include "abstract_list.h"
#include "abstract_value.h"
#include "make.h"

namespace rainynite::core {

/**
 * Typed storage of value node links.
 */
template <typename... Ts>
class LinkStorage : public AbstractListLinked
{
public:
    size_t link_count() const override {
        return sizeof...(Ts);
    }

    vector<AbstractReference> get_links() const override {
        return { storage.begin(), storage.end() };
    }

    AbstractReference get_link(size_t i) const override {
        check_range(i);
        return storage[i];
    }

    TypeConstraint get_link_type(size_t i) const override {
        check_range(i);
        return types()[i];
    }

    void set_link(size_t i, AbstractReference value) override {
        check_range(i);
        auto const& t = types()[i];
        if (!(t.accept(value->get_type())))
            throw NodeAccessError("Node property type mis-match");
        storage[i] = value;
    }

    template <typename... Is>
    void init_values(Is&&... values) {
        static_assert(sizeof...(Is) == sizeof...(Ts));
        storage = {make_value<Is>(std::forward<Is>(values))...};
        size_t i = 0;
        for (auto t : {std::type_index(typeid(Is))...}) {
            if (!types()[i].accept(t))
                throw NodeAccessError("Invalid type in init");
            ++i;
        }
    }

private:
    void check_range(size_t id) const {
        if (id >= sizeof...(Ts))
            throw std::out_of_range("set_link out of range");
    }

private:
    using TypeArray = array<TypeConstraint, sizeof...(Ts)>;
    static TypeArray const& types() {
        static TypeArray instance { TypeConstraint(Ts())... };
        return instance;
    }

    array<shared_ptr<AbstractValue>, sizeof...(Ts)> storage;
};

} // namespace rainynite::core

#endif
