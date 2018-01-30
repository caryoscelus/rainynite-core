/*  operators.cpp - standard c++ operators node templates
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

#include <functional>

#include <core/std/traits.h>
#include <core/node_info/macros.h>
#include <core/node_info/default_node.h>
#include <core/node/new_node.h>
#include <core/all_types.h>
#include <core/context.h>

namespace rainynite::core::nodes {

namespace detail {

template <typename Op, typename A>
using has_operator_t = decltype(Op()(declval<A&>(), declval<A&>()));

}

template <typename Op, typename A>
constexpr bool has_operator = is_detected_v<detail::has_operator_t, Op, A>;


template <typename R, typename T, template <typename> typename Op>
class BinaryNode :
    public NewNode<
        BinaryNode<R, T, Op>,
        R,
        types::Only<T>,
        types::Only<T>
    >
{
    NODE_PROPERTIES("a", "b")
    COMPLEX_DEFAULT_VALUES(make_default_node<T>(), make_default_node<T>())
    PROPERTY(a)
    PROPERTY(b)

    R get(shared_ptr<Context> ctx) const override {
        if constexpr (has_operator<Op<void>, T>) {
            return Op<void>()(a_value<T>(ctx), b_value<T>(ctx));
        } else {
            throw std::logic_error("Unsupported operator for this type.");
        }
    }
};

template <typename T>
using Add = BinaryNode<T,T,std::plus>;
NODE_INFO_INSTANCES(Add, Add<T>, T)

template <typename T>
using Sub = BinaryNode<T,T,std::minus>;
NODE_INFO_INSTANCES(Sub, Sub<T>, T)

template <typename T>
using Less = BinaryNode<bool,T,std::less>;
NODE_INFO_INSTANCES(Less, Less<T>, bool)

// template <typename T>
// using Mul = BinaryNode<T,std::multiplies>;
// NODE_INFO_INSTANCES(Mul, Mul<T>, T)
//
// template <typename T>
// using Div = BinaryNode<T,std::divides>;
// NODE_INFO_INSTANCES(Div, Div<T>, T)
//
// template <typename T>
// using Mod = BinaryNode<T,std::modulus>;
// NODE_INFO_INSTANCES(Mod, Mod<T>, T)

} // namespace rainynite::core::nodes
