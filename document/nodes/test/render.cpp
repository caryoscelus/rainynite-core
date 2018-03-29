/*  nodes/test/render.cpp - return result of rendering its child
 *  Copyright (C) 2018 caryoscelus
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
#include <core/node_info/default_node.h>
#include <core/node/new_node.h>
#include <core/renderable.h>
#include <core/renderers/svg_renderer.h>

namespace rainynite::core::nodes {

class SubRender :
    public NewNode<
        SubRender,
        string,
        types::Only<Renderable>,
        types::Only<string>
    >
{
    DOC_STRING(
        "Render a child node.\n"
        "\n"
        "This node calls renderer and returns results in some form."
        "The most common application is to render something into svg."
        "Can be used for testing.\n"
        "\n"
        "NOTE: this node is likely to change behaviour in future."
    )

    NODE_PROPERTIES("source", "mode")
    COMPLEX_DEFAULT_VALUES(make_default_node<Renderable>(), make_value<string>("svg"))
    PROPERTY(source)
    PROPERTY(mode)

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto mode = mode_value<string>(ctx);
        if (mode != "svg")
            return "Unsupported mode!";
        // TODO
        return renderers::node_to_svg({p_source(), ctx}, {});
    }
};

REGISTER_NODE(SubRender);

} // namespace rainynite::core::nodes
