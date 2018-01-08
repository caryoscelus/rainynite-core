/*  color_mix.cpp - color mixing node
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
#include <core/color/mix.h>

namespace rainynite::core::nodes {

class ColorMix :
    public NewNode<
        ColorMix,
        colors::Color,
        types::Only<colors::Color>,
        types::Only<colors::Color>,
        types::Only<double>,
        types::Only<bool>
    >
{
    DOC_STRING(
        "Mix two colors.\n"
        "\n"
        "Can be used as average node for Interpolate.\n"
    )

    NODE_PROPERTIES("a", "b", "progress", "strict")
    DEFAULT_VALUES(colors::Color{}, colors::Color{}, 0.0, false);

    PROPERTY(a)
    PROPERTY(b)
    PROPERTY(progress)
    PROPERTY(strict)

protected:
    colors::Color get(shared_ptr<Context> ctx) const override {
        using namespace colors;
        auto a = a_value<Color>(ctx);
        auto b = b_value<Color>(ctx);
        auto t = progress_value<double>(ctx);
        auto style =
            strict_value<bool>(ctx) ?
                ConversionStyle::Strict :
                ConversionStyle::NonStrict;
        return mix(a, b, t, style);
    }

};

REGISTER_NODE(ColorMix);

} // namespace rainynite::core::nodes
