/*  std/optional.h - re-import optional
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

#ifndef CORE_STD_OPTIONAL_H_A82C6819_AC9A_5AAA_B91D_8B1963730F16
#define CORE_STD_OPTIONAL_H_A82C6819_AC9A_5AAA_B91D_8B1963730F16

#if __has_include(<optional>)
#include <optional>

namespace rainynite {

using std::optional;
using std::make_optional;

} // namespace rainynite

#else

#include <experimental/optional>

namespace rainynite {

using std::experimental::optional;
using std::experimental::make_optional;

} // namespace rainynite

#endif



#endif
