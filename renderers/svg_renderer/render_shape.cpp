/*
 *  render_shape.cpp - SvgRenderer shape renderer
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

#include <string>

#include <fmt/format.h>

#include <core/color.h>
#include "svg_module.h"
#include "shape.h"

#include <geom_helpers/knots.h>
#include <geom_helpers/null_shape.h>

using namespace fmt::literals;

namespace rainynite::core {
namespace renderers {

class ShapeSvgRenderer : SVG_RENDERER_MODULE_CLASS(ShapeSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("RenderShape");
public:
    std::string operator()(AbstractNode const& node, std::shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto shape = node.get_property("shape")->get_any(ctx);
        auto color = node.get_property_as<colors::Color>("fill_color")->get(ctx);
        auto extra_style = get_extra_style(node, ctx, settings);
        return fmt::format(
            render_svg_shape(shape),
            "fill_color"_a=colors::to_hex24(color),
            "fill_opacity"_a=color.alpha(),
            "svg_style"_a=extra_style
        );
    }
};

} // namespace renderers
} // namespace rainynite::core
