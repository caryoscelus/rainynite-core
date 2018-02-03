/*  composite.cpp - Simple composite render node
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
#include <core/node/list.h>
#include <core/node_info/macros.h>

namespace rainynite::core::nodes {

class Composite :
    public RenderableNode<
        Composite,
        types::Only<vector<Renderable>>
    >
{
    DOC_STRING(
        "Composite list of renderables on top of each other.\n"
        "\n"
        "Note that this node may be deprecated in favor of more flexible"
        "approach where each \"Layer\" uses its own composite mode."
    )

    NODE_PROPERTIES("layers")
    COMPLEX_DEFAULT_VALUES(make_node<ListValue<Renderable>>())
    PROPERTY(layers)

public:
    bool can_set_source(shared_ptr<AbstractValue> src) const override {
        return src->get_type() == this->get_type();
    }
    void set_source(shared_ptr<AbstractValue> src) override {
        dynamic_cast<AbstractListLinked*>(p_layers().get())->push_back(src);
    }
};

REGISTER_NODE(Composite);

} // namespace rainynite::core::nodes
