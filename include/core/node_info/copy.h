/*  node_info/copy.h - shallow & deep node copu
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

#ifndef CORE_NODE_INFO_COPY_H_DCCF7E6E_7281_5C9C_A9CD_504E1FD4F0E0
#define CORE_NODE_INFO_COPY_H_DCCF7E6E_7281_5C9C_A9CD_504E1FD4F0E0

#include <core/node/common.h>

namespace rainynite::core {

AbstractReference shallow_copy(AbstractValue const& source);
AbstractReference deep_copy(AbstractValue const& source);

template <class T>
shared_ptr<T> shallow_copy_as(AbstractValue const& source) {
    return dynamic_pointer_cast<T>(shallow_copy(source));
}

} // namespace rainynite::core

#endif
