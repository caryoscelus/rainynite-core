/*  text.cpp - SvgRenderer text renderer
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

#include <core/std/string.h>

#include <fmt/format.h>

#include <core/color/color.h>
#include "svg_module.h"

namespace rainynite::core::renderers {

class TextSvgRenderer : SVG_RENDERER_MODULE_CLASS(TextSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Text");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto text = node.get_property_as<string>("text")->value(ctx);
        auto size = node.get_property_as<double>("size")->value(ctx);
        auto color = node.get_property_as<colors::Color>("color")->value(ctx);
        auto extra_style = get_extra_svg(node, ctx, settings, "style");
        return fmt::format(
            R"(<text x="0" y="0" font-size="{size}px" fill="{fill}" style="{svg_style}">{text}</text>)",
            "size"_a=size,
            "fill"_a=to_hex24(color),
            "svg_style"_a=extra_style,
            "text"_a=text
        );
    }
};

} // namespace rainynite::core::renderers
