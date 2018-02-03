/*  math.cpp - math operations nodes
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

#include <cmath>

#include <boost/math/constants/constants.hpp>

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

using boost::math::double_constants::pi;

namespace rainynite::core::nodes {


class Multiply :
    public NewNode<
        Multiply,
        double,
        types::Only<double>,
        types::Only<double>
    >
{
    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(0.0, 0.0)
    PROPERTY(a)
    PROPERTY(b)

protected:
    double get(shared_ptr<Context> ctx) const override {
        return a_value<double>(ctx) * b_value<double>(ctx);
    }
};

REGISTER_NODE(Multiply);


class Mod :
    public NewNode<
        Mod,
        double,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Take modulus"
    )

    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(0.0, 1.0)

    PROPERTY(a)
    PROPERTY(b)

protected:
    double get(shared_ptr<Context> ctx) const override {
        auto a = a_value<double>(ctx);
        if (auto b = b_value<double>(ctx))
            return std::fmod(a, b);
        return a;
    }
};

REGISTER_NODE(Mod);


class Sin :
    public NewNode<
        Sin,
        double,
        types::Only<double>
    >
{
    DOC_STRING("Sinus (angle in turns!)")

    NODE_PROPERTIES("turns")
    DEFAULT_VALUES(0.0)
    PROPERTY(turns)

protected:
    double get(shared_ptr<Context> ctx) const override {
        return std::sin(turns_value<double>(ctx) * pi * 2);
    }
};

REGISTER_NODE(Sin);


} // namespace rainynite::core::nodes
