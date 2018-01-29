/*  util/detect.h - some compile-time feature detection
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

#ifndef CORE_UTIL_DETECT_H_F3BCAC1D_DDEC_510D_9F40_90FF43051CE6
#define CORE_UTIL_DETECT_H_F3BCAC1D_DDEC_510D_9F40_90FF43051CE6

#include <core/std/traits.h>

#define DETECT_HAS_OPERATOR_WITH_SELF(name, OP) \
    template <typename T> \
    using name##_helper_t = decltype(declval<T&>() OP declval<T&>()); \
    template <typename T> \
    constexpr bool name = is_detected_v<name##_helper_t, T>

#define DETECT_HAS_OPERATOR_AB(name, OP) \
    template <typename A, typename B> \
    using name##_helper_t = decltype(declval<A&>() OP declval<B&>()); \
    template <typename A, typename B> \
    constexpr bool name = is_detected_v<name##_helper_t, A, B>

#endif
