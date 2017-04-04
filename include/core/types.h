/*
 *  types.h - runtime primitive type information
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

#ifndef __CORE__TYPES_H__0EBC3D4C
#define __CORE__TYPES_H__0EBC3D4C

#include <core/class_init.h>

namespace core {

class TypeInfo {
public:
    virtual std::string operator()() const = 0;
    virtual boost::any parse_string(std::string const& s) const = 0;
};

inline TypeInfo const& get_primitive_type(std::string const& name) {
    auto type = class_init::find_type(name);
    return class_init::type_meta<TypeInfo>(type);
}

template <typename T>
T parse_primitive_type(std::string const& type, std::string const& str) {
    return boost::any_cast<T>(
        get_primitive_type(type).parse_string(str)
    );
}

} // namespace core

#endif
