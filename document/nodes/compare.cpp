/*  nodes/compare.cpp - comparator nodes
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

#include <experimental/type_traits>

#include <core/node_info.h>
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/all_types.h>

namespace rainynite::core::nodes {

namespace detail {

using std::declval;
using std::experimental::is_detected_v;

template <class C>
using operator_eq_t = decltype(declval<C&>() == declval<C&>());

template <class C>
constexpr bool has_operator_eq = is_detected_v<operator_eq_t, C>;

}

template <typename T>
class Equal : public Node<bool> {
    DOC_STRING(
        "Node that compares its arguments."
    )
public:
    Equal() {
        this->template init<T>(a, {});
        this->template init<T>(b, {});
    }
public:
    bool get(shared_ptr<Context> ctx) const override {
        if constexpr (detail::has_operator_eq<T>) {
            return get_a()->value(ctx) == get_b()->value(ctx);
        } else {
            throw std::logic_error("Comparing incomparable type");
        }
    }

private:
    NODE_PROPERTY(a, T);
    NODE_PROPERTY(b, T);
};

NODE_INFO_TEMPLATE(Equal, Equal<T>, T);
TYPE_INSTANCES(EqualNodeInfo)

} // namespace rainynite::core::nodes
