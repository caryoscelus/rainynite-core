/*  image.cpp - Image import render node
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

#include <2geom/point.h>

namespace rainynite::core::nodes {

class Image :
    public RenderableNode<
        Image,
        types::Only<string>,
        types::Only<Geom::Point>
    >
{
    DOC_STRING(
        ""
    )

    NODE_PROPERTIES("file_path", "size")
    DEFAULT_VALUES(string{}, Geom::Point{})
    PROPERTY(file_path)
    PROPERTY(size)
};

REGISTER_NODE(Image);

} // namespace rainynite::core::nodes
