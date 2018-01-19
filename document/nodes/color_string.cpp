/*  color_string.cpp - convert color to string
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/color/color.h>

namespace rainynite::core::nodes {

class ColorString :
    public NewNode<
        ColorString,
        string,
        types::Only<colors::Color>
    >
{
    DOC_STRING(
        "Converts color to hex24 string.\n"
        "\n"
        "TODO: add more output options"
    )

    // TODO: rename color to source?
    NODE_PROPERTIES("color")
    DEFAULT_VALUES(colors::Color{})
    PROPERTY(color)

protected:
    string get(shared_ptr<Context> ctx) const override {
        return colors::to_hex24(color_value<colors::Color>(ctx));
    }
};

REGISTER_NODE(ColorString);

} // namespace rainynite::core::nodes
