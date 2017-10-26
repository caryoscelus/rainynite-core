/*  color_mix.cpp - color mixing node
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
#include <core/color.h>

namespace rainynite::core::nodes {

class ColorMix : public Node<colors::Color> {
    DOC_STRING(
        "Mix two colors.\n"
        "\n"
        "Can be used as average node for Interpolate.\n"
        "\n"
        "NOTE: can be deprecated in favor of generic average node."
    )
public:
    ColorMix() {
        init<colors::Color>(a, {});
        init<colors::Color>(b, {});
        init<double>(progress, 0);
    }
public:
    colors::Color get(shared_ptr<Context> ctx) const override {
        auto a = get_a()->get(ctx);
        auto b = get_b()->get(ctx);
        auto t = get_progress()->get(ctx);
        return colors::mix(a, b, t);
    }

private:
    NODE_PROPERTY(a, colors::Color);
    NODE_PROPERTY(b, colors::Color);
    NODE_PROPERTY(progress, double);
};

REGISTER_NODE(ColorMix);

} // namespace rainynite::core::nodes
