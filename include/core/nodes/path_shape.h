/*
 *  path_shape.h - Path Shape render node
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

#ifndef __CORE__NODES__PATH_SHAPE_H__D0F028BA
#define __CORE__NODES__PATH_SHAPE_H__D0F028BA

#include <core/node.h>
#include <core/node_info.h>
#include <core/renderable.h>

#include <geom_helpers/knots.h>

namespace core {
namespace nodes {

class PathShape : public Node<Renderable> {
public:
    PathShape() {
        init_list<Geom::BezierKnots>(path);
    }
public:
    virtual Renderable get(Time time) const override {
        return Renderable();
    }
private:
    NODE_PROPERTY(path, Geom::BezierKnots);
};

REGISTER_NODE(PathShape);

} // namespace nodes
} // namespace core

#endif
