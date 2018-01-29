/*  std/traits.h - re-import standard traits
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

#ifndef CORE_STD_TRAITS_H_AC7BDEFD_C75E_535D_BBC4_D867D68F4209
#define CORE_STD_TRAITS_H_AC7BDEFD_C75E_535D_BBC4_D867D68F4209

#include <type_traits>
#include <experimental/type_traits>
#include <utility>

namespace rainynite {

using std::is_same;
using std::experimental::is_same_v;

using std::is_base_of;
using std::experimental::is_base_of_v;

using std::experimental::is_detected;
using std::experimental::is_detected_v;

using std::declval;

} // namespace rainynite

#endif
