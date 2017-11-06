/*
 *  text.cpp - SvgRenderer text renderer
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

namespace rainynite::core {
namespace renderers {

const string svg_text = R"(<text x="0" y="0" font-size="{}px" fill="{}">{}</text>)";

class TextSvgRenderer : SVG_RENDERER_MODULE_CLASS(TextSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Text");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& /*settings*/) const override {
        auto text = node.get_property_as<string>("text")->get(ctx);
        auto size = node.get_property_as<double>("size")->get(ctx);
        auto color = node.get_property_as<colors::Color>("color")->get(ctx);
        return fmt::format(svg_text, size, to_hex24(color), text);
    }
};

} // namespace renderers
} // namespace rainynite::core
