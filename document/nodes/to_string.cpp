/*  to_string.cpp - node converting any value to string
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
#include <core/serialize/node_writer.h>

namespace rainynite::core::nodes {

class ToString :
    public NewNode<
        ToString,
        string,
        types::Any
    >
{
    NODE_PROPERTIES("source")
    DEFAULT_VALUES(string{})
    PROPERTY(source)

    DOC_STRING(
        "Convert any type to string..."
    )

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto v = p_source()->get_any(ctx);
        return serialize::value_to_string(v);
    }
};

REGISTER_NODE(ToString);

} // namespace rainynite::core::nodes
