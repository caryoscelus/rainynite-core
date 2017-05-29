/*
 *  make.h - Node-making functions
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

#ifndef __CORE__NODE__MAKE_H__A6087C20
#define __CORE__NODE__MAKE_H__A6087C20

#include <memory>

#include "value.h"

namespace core {

template <typename T, typename... Args>
std::shared_ptr<Value<T>> make_value(Args&&... args) {
    auto r = std::make_shared<Value<T>>();
    r->new_id();
    r->emplace(std::forward<Args>(args)...);
    return r;
}

template <class T>
std::shared_ptr<T> make_node() {
    auto r = std::make_shared<T>();
    r->new_id();
    return r;
}

} // namespace core

#endif
