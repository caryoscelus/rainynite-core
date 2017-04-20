/*
 *  path_shape.cpp - Path Shape render node
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
#include <core/color.h>
#include <core/renderable.h>

#include <geom_helpers/knots.h>

namespace core {
namespace nodes {

class PathShape : public Node<Renderable> {
public:
    PathShape() {
        init<Geom::BezierKnots>(path, Geom::BezierKnots());
        init<colors::Color>(fill_color, colors::Color());
        init<std::string>(extra_style, "");
    }
public:
    virtual Renderable get(Time time) const override {
        return Renderable();
    }
private:
    NODE_PROPERTY(path, Geom::BezierKnots);
    NODE_PROPERTY(fill_color, colors::Color);
    NODE_PROPERTY(extra_style, std::string);
};

REGISTER_NODE(PathShape);

} // namespace nodes
} // namespace core
