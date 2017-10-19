/*  make.h - Node-making functions
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

#ifndef CORE_NODE_MAKE_H_C211DBC5_E4F0_54CF_BD3E_EFA07946E7B1
#define CORE_NODE_MAKE_H_C211DBC5_E4F0_54CF_BD3E_EFA07946E7B1

#include <core/std/memory.h>

#include "value.h"

namespace rainynite::core {

/**
 * Make value node.
 *
 * Optional arguments are passed to T ctor.
 */
template <typename T, typename... Args>
shared_ptr<Value<T>> make_value(Args&&... args) {
    auto r = make_shared<Value<T>>();
    r->new_id();
    r->emplace(std::forward<Args>(args)...);
    return r;
}

/**
 * Make node of given type and init it's id.
 *
 * This should either be used or thrown out.
 */
template <class T>
shared_ptr<T> make_node() {
    auto r = make_shared<T>();
    r->new_id();
    return r;
}

} // namespace rainynite::core

#endif
