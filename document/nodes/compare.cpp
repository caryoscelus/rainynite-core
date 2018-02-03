/*  nodes/compare.cpp - comparator nodes
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

#include <experimental/type_traits>
#include <limits>

#include <boost/math/special_functions/relative_difference.hpp>

#include <core/node_info/macros.h>
#include <core/node_info/default_node.h>
#include <core/node/proxy_node.h>
#include <core/node/new_node.h>
#include <core/all_types.h>
#include <core/util/detect.h>

namespace rainynite::core::nodes {

namespace detail {

DETECT_HAS_OPERATOR_WITH_SELF(has_operator_eq, ==);

} // namespace detail

template <typename T>
class Equal :
    public NewNode<
        Equal<T>,
        bool,
        types::Only<T>,
        types::Only<T>
    >
{
    DOC_STRING(
        "Node that compares its arguments."
    )

    NODE_PROPERTIES("a", "b")
    COMPLEX_DEFAULT_VALUES(make_default_node<T>(), make_default_node<T>())
    PROPERTY(a)
    PROPERTY(b)

protected:
    bool get(shared_ptr<Context> ctx) const override {
        if constexpr (detail::has_operator_eq<T>) {
            return a_value<T>(ctx) == b_value<T>(ctx);
        } else {
            throw std::logic_error("Comparing incomparable type");
        }
    }
};

NODE_INFO_TEMPLATE(Equal, Equal<T>, bool);
TYPE_INSTANCES(EqualNodeInfo)


class FuzzyEqual :
    public NewNode<
        FuzzyEqual,
        bool,
        types::Only<double>,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Return whether two real numbers are equal within error margin"
    )

    NODE_PROPERTIES("a", "b", "relative_eps")
    DEFAULT_VALUES(0.0, 0.0, std::numeric_limits<double>::epsilon()*16)
    PROPERTY(a)
    PROPERTY(b)
    PROPERTY(relative_eps)

protected:
    bool get(shared_ptr<Context> ctx) const override {
        using boost::math::relative_difference;
        auto diff = relative_difference(a_value<double>(ctx), b_value<double>(ctx));
        return diff < relative_eps_value<double>(ctx);
    }
};

REGISTER_NODE(FuzzyEqual);


class TestLinksAreEqual :
    public NewNode<
        TestLinksAreEqual,
        bool,
        types::Any,
        types::Any
    >
{
    DOC_STRING(
        "Checks whether it's children are same node.\n"
        "\n"
        "This is really only useful for testing serialization."
    )

    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(Nothing{}, Nothing{});

    PROPERTY(a)
    PROPERTY(b)

protected:
    bool get(shared_ptr<Context> /*ctx*/) const override {
        return p_a() == p_b();
    }
};

REGISTER_NODE(TestLinksAreEqual);


} // namespace rainynite::core::nodes
