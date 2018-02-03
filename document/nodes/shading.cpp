/*  shading.cpp - shading-related nodes
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
#include <core/shading.h>

namespace rainynite::core::nodes {

class ShadingStyle :
    public NewNode<
        ShadingStyle,
        Shading,
        types::Only<colors::Color>,
        types::Only<colors::Color>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Shading style."
    )

    NODE_PROPERTIES("fill_color", "line_color", "line_width")
    DEFAULT_VALUES(colors::Color{}, colors::Color{}, 1.0)
    PROPERTY(fill_color)
    PROPERTY(line_color)
    PROPERTY(line_width)

protected:
    Shading get(shared_ptr<Context> ctx) const override {
        return {
            fill_color_value<colors::Color>(ctx),
            line_color_value<colors::Color>(ctx),
            line_width_value<double>(ctx),
            ""
        };
    }
};

REGISTER_NODE(ShadingStyle);

} // namespace rainynite::core::nodes
