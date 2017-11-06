/*
 *  color_string.cpp - convert color to string
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/color/color.h>

namespace rainynite::core {
namespace nodes {

class ColorString : public Node<string> {
public:
    ColorString() {
        init<colors::Color>(color, {});
    }
public:
    string get(shared_ptr<Context> ctx) const override {
        try {
            auto c = get_color()->get(ctx);
            return colors::to_hex24(c);
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(color, colors::Color);
};

REGISTER_NODE(ColorString);

} // namespace nodes
} // namespace rainynite::core
