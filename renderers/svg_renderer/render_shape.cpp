/*  render_shape.cpp - SvgRenderer shape renderer
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

#include <core/shading.h>
#include "svg_module.h"
#include "shape.h"

using namespace fmt::literals;

namespace rainynite::core::renderers {

const string styled_svg_element = R"(<{shape} style="fill:{fill_color};fill-opacity:{fill_opacity};stroke:{line_color};stroke-opacity:{line_opacity};{svg_style}" />)";

class ShapeSvgRenderer : SVG_RENDERER_MODULE_CLASS(ShapeSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("RenderShape");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto shape = node.get_property("shape")->get_any(ctx);
        auto shading_node = node.get_property_as<Shading>("shading");
        auto shading = shading_node->value(ctx);
        auto extra_style = get_extra_style(shading_node, ctx, settings);
        return fmt::format(
            styled_svg_element,
            "shape"_a=render_svg_shape(shape),
            "fill_color"_a=colors::to_hex24(shading.fill_color),
            "fill_opacity"_a=shading.fill_color.alpha(),
            "line_color"_a=colors::to_hex24(shading.line_color),
            "line_opacity"_a=shading.line_color.alpha(),
            "svg_style"_a=extra_style
        );
    }
};

} // namespace rainynite::core::renderers
