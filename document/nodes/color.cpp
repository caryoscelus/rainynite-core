/*  color.cpp - color operations
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
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/color/color.h>

namespace rainynite::core::nodes {

class InvertColor : public Node<colors::Color> {
    DOC_STRING(
        "Invert color. Doesn't touch opacity."
    )

public:
    InvertColor() {
        init<colors::Color>(source, {});
    }

protected:
    colors::Color get(shared_ptr<Context> ctx) const override {
        auto c = get_source()->value(ctx);
        // TODO: make it lib function
        return colors::Color::from_rgba(
            1.0-c.red(),
            1.0-c.green(),
            1.0-c.blue(),
            c.alpha()
        );
    }

private:
    NODE_PROPERTY(source, colors::Color);
};

REGISTER_NODE(InvertColor);

} // namespace rainynite::core::nodes
