/*  rectangle.cpp - rectangle constructing nodes
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

#include <geom_helpers/rectangle.h>

namespace rainynite::core::nodes {

class RectangleWH :
    public NewNode<
        RectangleWH,
        Geom::Rectangle,
        types::Only<Geom::Point>,
        types::Only<Geom::Point>
    >
{
    DOC_STRING("Make rectangle from position & size")

    NODE_PROPERTIES("position", "size")
    DEFAULT_VALUES(Geom::Point{}, Geom::Point{})
    PROPERTY(position)
    PROPERTY(size)

protected:
    Geom::Rectangle get(shared_ptr<Context> ctx) const override {
        using Geom::Point;
        return {position_value<Point>(ctx), size_value<Point>(ctx)};
    }
};

REGISTER_NODE(RectangleWH);


class RectangleAB :
    public NewNode<RectangleAB,
        Geom::Rectangle,
        types::Only<Geom::Point>,
        types::Only<Geom::Point>
    >
{

    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(Geom::Point{}, Geom::Point{})
    PROPERTY(a)
    PROPERTY(b)

protected:
    Geom::Rectangle get(shared_ptr<Context> ctx) const override {
        using Geom::Point, Geom::Rectangle;
        return Rectangle::fromTwoPoints(
            a_value<Point>(ctx),
            b_value<Point>(ctx)
        );
    }
};

REGISTER_NODE(RectangleAB);


} // namespace rainynite::core::nodes
