/*  svg.cpp - svg import node
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

#include <core/renderable.h>
#include <core/node/new_node.h>
#include <core/node_info.h>

namespace rainynite::core::nodes {

class EmbedSvg :
    public NewNode<
        EmbedSvg,
        Renderable,
        types::Only<string>
    >
{
    DOC_STRING(
        "Embed svg document."
    )

    NODE_PROPERTIES("file_path")
    DEFAULT_VALUES(string{})

    PROPERTY(file_path)

public:
    Renderable get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
};

REGISTER_NODE(EmbedSvg);

} // namespace rainynite::core::nodes
