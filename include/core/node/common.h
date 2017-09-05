/*
 *  common.h - Node-related forward declarations and typedefs
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

#ifndef __CORE__NODE__COMMON_H__7A32160E
#define __CORE__NODE__COMMON_H__7A32160E

#include <stdexcept>

#include <core/std/type.h>
#include <core/std/memory.h>
#include <core/std/optional.h>
#include <core/std/string.h>

namespace rainynite::core {

// forward declarations
class AbstractValue;

template <typename T>
class BaseValue;

// type aliases
using Type = std::type_index;

using AbstractReference = shared_ptr<AbstractValue>;

template <typename T>
using BaseReference = shared_ptr<BaseValue<T>>;

// exceptions
class NodeAccessError : public std::runtime_error {
public:
    NodeAccessError(string const& msg) :
        std::runtime_error(msg)
    {}
};

} // namespace rainynite::core

#endif
