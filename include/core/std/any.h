/*  std/any.h - re-import any
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

#ifndef CORE_STD_ANY_H_386E0F76_7456_5B41_872A_3D71D497E43F
#define CORE_STD_ANY_H_386E0F76_7456_5B41_872A_3D71D497E43F

#if __has_include(<any>)
# include <any>
# define ANY_NS std
#elif __has_include(<experimental/any>)
# include <experimental/any>
# define ANY_NS std::experimental
#else
# include <boost/any.hpp>
# define ANY_NS boost
#endif

#include "traits.h"

namespace rainynite {

using ANY_NS::any;
using ANY_NS::any_cast;

namespace detail {

using std::declval;

template <typename A>
using has_has_value_t = decltype(declval<A const&>().has_value());

static_assert(std::is_same_v<has_has_value_t<any>,bool>);

template <typename A>
using has_empty_t = decltype(declval<A const&>().empty());

template <typename A>
constexpr bool type_has_has_value = is_detected_v<has_has_value_t, A>;

template <typename A>
constexpr bool type_has_empty = is_detected_v<has_empty_t, A>;

template <typename A, typename R>
constexpr R any_has_value(A const& obj);

template <typename A>
constexpr std::enable_if_t<type_has_has_value<A>, bool> any_has_value(A const& obj) {
    return obj.has_value();
}

template <typename A>
constexpr std::enable_if_t<type_has_empty<A>, bool> any_has_value(A const& obj) {
    return !obj.empty();
}

} // namespace detail

inline bool any_has_value(any const& obj) noexcept {
    return detail::any_has_value(obj);
}

} // namespace rainynite

#endif
