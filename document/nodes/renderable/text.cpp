/*  text.cpp - Text render node
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
#include <core/color/color.h>

namespace rainynite::core::nodes {

class Text :
    public RenderableNode<
        Text,
        types::Only<string>,
        types::Only<colors::Color>,
        types::Only<double>
    >
{
    DOC_STRING("Render text")

    NODE_PROPERTIES("text", "color", "size")
    DEFAULT_VALUES(string{}, colors::Color{}, 1.0)

    PROPERTY(text)
    PROPERTY(color)
    PROPERTY(size)
};

REGISTER_NODE(Text);

} // namespace rainynite::core::nodes
