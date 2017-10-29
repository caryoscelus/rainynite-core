/*  named_link_storage.h - link storage
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

#ifndef CORE_NODE_NAMED_LINK_STORAGE_H_067842B0_98D7_5D5D_B8A3_0C1D771E99AC
#define CORE_NODE_NAMED_LINK_STORAGE_H_067842B0_98D7_5D5D_B8A3_0C1D771E99AC

#include <core/std/map.h>
#include "link_storage.h"

namespace rainynite::core {

inline map<string, size_t> get_name_map(vector<string> const& names) {
    // TODO: algorithm?
    map<string, size_t> result;
    size_t i = 0;
    for (auto const& s : names) {
        result[s] = i;
        ++i;
    }
    return result;
}

template <class Self, typename... Ts>
class NamedLinkStorage :
    public LinkStorage<Ts...>
{
public:
    static size_t get_name_id(string const& name) {
        auto const& names = name_map();
        auto it = names.find(name);
        if (it != names.end())
            return it->second;
        throw NodeAccessError("Property doesn't exist");
    }

    AbstractReference get_property(string const& name) const {
        return this->get_link(get_name_id(name));
    }

    template <typename T>
    shared_ptr<BaseValue<T>> get_property_as(string const& name) const {
        return dynamic_pointer_cast<BaseValue<T>>(get_property(name));
    }

    void set_property(string const& name, AbstractReference ref) {
        this->set_link(get_name_id(name), ref);
    }

private:
    static map<string, size_t> const& name_map() {
        static map<string, size_t> instance(get_name_map(Self::link_names()));
        return instance;
    }
};

} // namespace rainynite::core

#endif
