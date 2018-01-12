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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/color/color.h>

namespace rainynite::core::nodes {

class InvertColor :
    public NewNode<
        InvertColor,
        colors::Color,
        types::Only<colors::Color>
    >
{
    DOC_STRING(
        "Invert color. Doesn't touch opacity."
    )

    NODE_PROPERTIES("source")
    DEFAULT_VALUES(colors::Color{})

    PROPERTY(source)

protected:
    colors::Color get(shared_ptr<Context> ctx) const override {
        auto c = source_value<colors::Color>(ctx);
        // TODO: make it lib function
        return colors::Color::from_rgba(
            1.0-c.red(),
            1.0-c.green(),
            1.0-c.blue(),
            c.alpha()
        );
    }
};

REGISTER_NODE(InvertColor);


class HsvColor :
    public NewNode<
        HsvColor,
        colors::Color,
        types::Only<double>,
        types::Only<double>,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Color composed from hue, saturation, value (and alpha)"
    )

    NODE_PROPERTIES("hue", "saturation", "value", "alpha")
    DEFAULT_VALUES(0.0, 0.0, 0.0, 1.0)

    PROPERTY(hue)
    PROPERTY(saturation)
    PROPERTY(value)
    PROPERTY(alpha)

protected:
    colors::Color get(shared_ptr<Context> ctx) const override {
        return colors::Color::from_hsv(
            hue_value<double>(ctx),
            saturation_value<double>(ctx),
            value_value<double>(ctx),
            alpha_value<double>(ctx)
        );
    }
};

REGISTER_NODE(HsvColor);


class RgbColor :
    public NewNode<
        RgbColor,
        colors::Color,
        types::Only<double>,
        types::Only<double>,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Color composed from red, green, blue (and alpha)"
    )

    NODE_PROPERTIES("red", "green", "blue", "alpha")
    DEFAULT_VALUES(0.0, 0.0, 0.0, 1.0)

    PROPERTY(red)
    PROPERTY(green)
    PROPERTY(blue)
    PROPERTY(alpha)

protected:
    colors::Color get(shared_ptr<Context> ctx) const override {
        return colors::Color::from_rgba(
            red_value<double>(ctx),
            green_value<double>(ctx),
            blue_value<double>(ctx),
            alpha_value<double>(ctx)
        );
    }
};

REGISTER_NODE(RgbColor);


} // namespace rainynite::core::nodes
