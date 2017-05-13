/*
 *  shapes.cpp - SvgRenderer shape renderer
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

#include <geom_helpers/knots.h>

namespace core {
namespace renderers {

const std::string svg_path = R"(<path d="{}" style="fill:{};fill-opacity:{};stroke:none;{}" />)";

const std::string svg_rectangle = R"(<rect x="{}" y="{}" width="{}" height="{}" style="fill:{};fill-opacity:{};stroke:none;{}"/>)";

class PathShapeSvgRenderer : SVG_RENDERER_MODULE_CLASS(PathShapeSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("PathShape");
public:
    virtual std::string operator()(AbstractNode const& node, Time time, SvgRendererSettings const& settings) const override {
        auto path = node.get_property_as<Geom::BezierKnots>("path")->get(time);
        auto color = node.get_property_as<colors::Color>("fill_color")->get(time);
        auto extra_style = get_extra_style(node, time, settings);
        return fmt::format(svg_path, Geom::knots_to_svg(path), colors::to_hex24(color), color.alpha(), extra_style);
    }
};

class RectangleShapeSvgRenderer : SVG_RENDERER_MODULE_CLASS(RectangleShapeSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("RectangleShape");
public:
    virtual std::string operator()(AbstractNode const& node, Time time, SvgRendererSettings const& settings) const override {
        auto pos = node.get_property_as<Geom::Point>("position")->get(time);
        auto size = node.get_property_as<Geom::Point>("size")->get(time);
        auto color = node.get_property_as<colors::Color>("fill_color")->get(time);
        auto extra_style = get_extra_style(node, time, settings);
        return fmt::format(svg_rectangle, pos.x(), pos.y(), size.x(), size.y(), colors::to_hex24(color), color.alpha(), extra_style);
    }
};

} // namespace renderers
} // namespace core
