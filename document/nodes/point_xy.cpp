/*  point_xy.cpp - combine x&y to Point (see ExtractCoord for reverse)
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

#include <2geom/point.h>

namespace rainynite::core::nodes {

class PointXY :
    public NewNode<
        PointXY,
        Geom::Point,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Create Point from coordinate pair"
    )

    NODE_PROPERTIES("x", "y")
    DEFAULT_VALUES(0.0, 0.0)
    PROPERTY(x)
    PROPERTY(y)

protected:
    Geom::Point get(shared_ptr<Context> ctx) const override {
        return {x_value<double>(ctx), y_value<double>(ctx)};
    }
    bool can_set_any(any const& value) const override {
        return get_type() == value.type();
    }
    void set_any(any const& value) override {
        auto point = any_cast<Geom::Point>(value);
        set_property("x", make_value<double>(point.x()));
        set_property("y", make_value<double>(point.y()));
    }
};

REGISTER_NODE(PointXY);


class SymmetricPoint :
    public NewNode<
        SymmetricPoint,
        Geom::Point,
        types::Only<double>
    >
{
    DOC_STRING(
        "Create Point with equal coordinates"
    )

    NODE_PROPERTIES("x")
    DEFAULT_VALUES(0.0)
    PROPERTY(x)

protected:
    Geom::Point get(shared_ptr<Context> ctx) const override {
        auto x = x_value<double>(ctx);
        return {x, x};
    }
};

REGISTER_NODE(SymmetricPoint);

} // namespace rainynite::core::nodes
