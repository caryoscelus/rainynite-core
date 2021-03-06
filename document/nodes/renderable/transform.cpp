/*  transform.cpp - apply affine transformation to renderable
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

#include <core/renderable_node.h>
#include <core/node_info/macros.h>
#include <core/node_info/default_node.h>

#include <2geom/affine.h>

namespace rainynite::core::nodes {

class Transform :
    public RenderableNode<
        Transform,
        types::Only<Renderable>,
        types::Only<Geom::Affine>
    >
{
    DOC_STRING(
        "Apply affine transformation to renderable node."
    )

    NODE_PROPERTIES("source", "transform")
    COMPLEX_DEFAULT_VALUES(make_default_node<Renderable>(), make_value<Geom::Affine>());
    PROPERTY(source)
    PROPERTY(transform)
};

REGISTER_NODE(Transform);

} // namespace rainynite::core::nodes
