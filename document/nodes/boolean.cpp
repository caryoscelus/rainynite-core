/*  boolean.cpp - bool operation nodes
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

#include <core/node_info.h>
#include <core/node/new_node.h>

namespace rainynite::core::nodes {

class Not :
    public NewNode<
        Not,
        bool,
        types::Only<bool>
    >
{
    DOC_STRING(
        "Negate boolean"
    )

    NODE_PROPERTIES("source")
    DEFAULT_VALUES(true);

    PROPERTY(source)

protected:
    bool get(shared_ptr<Context> ctx) const override {
        return !source_value<bool>(ctx);
    }
};

REGISTER_NODE(Not);

} // namespace rainynite::core::nodes
