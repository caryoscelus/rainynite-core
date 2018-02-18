/*  util/type_info.h - runtime primitive type information
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

#ifndef CORE_TYPES_H_9E3A32A1_A9D0_52CB_AA5B_25CABBDD7A32
#define CORE_TYPES_H_9E3A32A1_A9D0_52CB_AA5B_25CABBDD7A32

#include <core/util/class_init.h>

namespace rainynite::core {

class TypeInfo {
public:
    virtual string operator()() const = 0;
    virtual any parse_string(string const& s) const = 0;
};

inline TypeInfo const& get_primitive_type(std::type_index type) {
    return class_init::type_meta<TypeInfo>(type);
}

inline TypeInfo const& get_primitive_type(string const& name) {
    auto type = class_init::find_type(name);
    return get_primitive_type(type);
}

inline string get_primitive_type_name(std::type_index type) {
    return class_init::type_info<TypeInfo,string>(type);
}

template <typename T>
string get_primitive_type_name() {
    return get_primitive_type_name(typeid(T));
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

/**
 * Returns all registered types.
 *
 * NOTE: if this is called during initialization, there is no way to ensure that
 * all types will be returned by this function.
 */
inline map<Type, TypeInfo*> const& all_types() {
    return class_init::class_registry<TypeInfo>();
}

} // namespace rainynite::core

#endif
