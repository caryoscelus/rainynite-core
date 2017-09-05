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

namespace rainynite::core {

class TypeInfo {
public:
    virtual string operator()() const = 0;
    virtual any parse_string(string const& s) const = 0;
};

#define TYPE_INFO(Type, name, parse) \
class Type##TypeInfo : \
    public rainynite::core::TypeInfo, \
    class_init::Registered<Type##TypeInfo, Type, rainynite::core::TypeInfo>, \
    class_init::ReverseRegistered<Type##TypeInfo, Type, rainynite::string> \
{ \
public: \
    rainynite::string operator()() const override { \
        return name; \
    } \
    rainynite::any parse_string(rainynite::string const& s) const override { \
        return parse(s); \
    } \
}

inline TypeInfo const& get_primitive_type(std::type_index type) {
    return class_init::type_meta<TypeInfo>(type);
}

inline TypeInfo const& get_primitive_type(string const& name) {
    auto type = class_init::find_type(name);
    return get_primitive_type(type);
}

template <typename T>
any parse_primitive_type(T type, string const& str) {
    return get_primitive_type(type).parse_string(str);
}

template <typename T, typename... Args>
T parse_primitive_type_to(Args&&... args) {
    return any_cast<T>(
        parse_primitive_type(std::forward<Args>(args)...)
    );
}

} // namespace rainynite::core

#endif
