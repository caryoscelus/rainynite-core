/*
 *  point_xy.cpp - combine x&y to Point (see ExtractCoord for reverse)
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

#include <2geom/point.h>

namespace core {
namespace nodes {

class PointXY : public Node<Geom::Point> {
public:
    PointXY() {
        init<double>(x, 0);
        init<double>(y, 0);
    }
public:
    Geom::Point get(std::shared_ptr<Context> ctx) const override {
        try {
            return {get_x()->get(ctx), get_y()->get(ctx)};
        } catch (...) {
            return {};
        }
    }
    virtual bool can_set_any(boost::any const& value) const override {
        return get_type() == value.type();
    }
    virtual void set_any(boost::any const& value) override {
        auto point = boost::any_cast<Geom::Point>(value);
        set_property("x", make_value<double>(point.x()));
        set_property("y", make_value<double>(point.y()));
    }

private:
    NODE_PROPERTY(x, double);
    NODE_PROPERTY(y, double);
};

REGISTER_NODE(PointXY);

} // namespace nodes
} // namespace core
