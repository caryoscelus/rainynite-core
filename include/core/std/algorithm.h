/*  std/algorithm.h - re-import algorithm library
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

#ifndef CORE_STD_ALGORITHM_H_026AB086_E6C0_5E4D_95E8_EF7702E23787
#define CORE_STD_ALGORITHM_H_026AB086_E6C0_5E4D_95E8_EF7702E23787

#include <algorithm>

#if __has_include(<experimental/algorithm>)
#include <experimental/algorithm>
#endif

namespace rainynite {

#ifdef __cpp_lib_clamp
using std::clamp;
#else
// using std::experimental::clamp; // apparently there is no such thing
template <typename T>
constexpr T const& clamp(T const& v, T const& lo, T const& hi) {
    return v <= lo ? lo : v >= hi ? hi : v;
}
#endif

} // namespace rainynite

#endif
