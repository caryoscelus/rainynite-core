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

#include <core/node.h>
#include <core/node_info.h>

#include <2geom/point.h>

namespace core {
namespace nodes {

REGISTER_NODE(PointXY);

class PointXY : public Node<Geom::Point> {
public:
    PointXY() {
        init<double>(x, 0);
        init<double>(y, 0);
    }
public:
    virtual Geom::Point get(Time time) const override {
        try {
            return {get_x()->get(time), get_y()->get(time)};
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(x, double);
    NODE_PROPERTY(y, double);
};

} // namespace nodes
} // namespace core
