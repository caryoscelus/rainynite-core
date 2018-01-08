/*  util/nullptr.h - nullptr helpers
 *  Copyright (C) 2018 caryoscelus
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

#ifndef CORE_NULLPTR_H_ABF94BE0_1FB9_53C3_AB84_A410231482C2
#define CORE_NULLPTR_H_ABF94BE0_1FB9_53C3_AB84_A410231482C2

#include "exceptions.h"

namespace rainynite {

template <typename P>
P no_null(P&& pointer) {
    if (pointer == nullptr)
        throw NullPointerException("Null pointer passed through no_null");
    return std::forward<P>(pointer);
}

} // namespace rainynite

#endif
