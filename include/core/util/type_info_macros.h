/*  util/type_info_macros.h - runtime primitive type information macros
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

#ifndef CORE_UTIL_TYPE_INFO_MACROS_H_88781968_31EA_5A0F_AB47_5EE34F2F9F68
#define CORE_UTIL_TYPE_INFO_MACROS_H_88781968_31EA_5A0F_AB47_5EE34F2F9F68

#include "type_info.h"

#define TYPE_INFO_NAMED(Instance, Type, name, parse) \
class Instance : \
    public rainynite::core::TypeInfo, \
    class_init::Registered<Instance, Type, rainynite::core::TypeInfo>, \
    class_init::ReverseRegistered<Instance, Type, rainynite::string> \
{ \
public: \
    rainynite::string operator()() const override { \
        return name; \
    } \
    rainynite::any parse_string(rainynite::string const& s) const override { \
        return parse(s); \
    } \
}

#define TYPE_INFO(Type, name, parse) \
    TYPE_INFO_NAMED(Type##TypeInfo, Type, name, parse)

#endif
