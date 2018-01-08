/*  shading.cpp - shading-related nodes
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

#include <core/node_info/macros.h>
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/shading.h>

namespace rainynite::core::nodes {

class ShadingStyle : public Node<Shading> {
    DOC_STRING(
        "Shading style."
    )

public:
    ShadingStyle() {
        init<colors::Color>(fill_color, {});
        init<colors::Color>(line_color, {});
        init<double>(line_width, 1.0);
    }

    Shading get(shared_ptr<Context> ctx) const override {
        return {
            get_fill_color()->value(ctx),
            get_line_color()->value(ctx),
            get_line_width()->value(ctx),
            ""
        };
    }

private:
    NODE_PROPERTY(fill_color, colors::Color);
    NODE_PROPERTY(line_color, colors::Color);
    NODE_PROPERTY(line_width, double);
};

REGISTER_NODE(ShadingStyle);

} // namespace rainynite::core::nodes
