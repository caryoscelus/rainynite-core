/*  cast.h - common node casts
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

#ifndef CORE_NODE_CAST_H_D35FC2FE_53C0_5F57_9E51_7B3FD5202D55
#define CORE_NODE_CAST_H_D35FC2FE_53C0_5F57_9E51_7B3FD5202D55

#include <core/std/memory.h>

namespace rainynite::core {

class AbstractValue;

template <typename T>
class BaseValue;

class AbstractListLinked;

class AbstractNode;

template <class From>
shared_ptr<AbstractValue> abstract_value_cast(From&& pointer) {
    return dynamic_pointer_cast<AbstractValue>(std::forward<From>(pointer));
}

template <typename T, class From>
shared_ptr<BaseValue<T>> base_value_cast(From&& pointer) {
    return dynamic_pointer_cast<BaseValue<T>>(std::forward<From>(pointer));
}

template <class From>
shared_ptr<AbstractListLinked> list_cast(From&& pointer) {
    return dynamic_pointer_cast<AbstractListLinked>(std::forward<From>(pointer));
}

template <class From>
shared_ptr<AbstractNode> abstract_node_cast(From&& pointer) {
    return dynamic_pointer_cast<AbstractNode>(std::forward<From>(pointer));
}

} // namespace rainynite::core

#endif
