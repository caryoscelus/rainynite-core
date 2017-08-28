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

#include <fmt/format.h>

#include "svg_module.h"
#include "shape.h"

#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/circle.h>
#include <geom_helpers/knots.h>

using namespace fmt::literals;

namespace core {
namespace renderers {

const std::string svg_path = R"(<path d="{path}" style="fill:{{fill_color}};fill-opacity:{{fill_opacity}};stroke:none;{{svg_style}}" />)";

const std::string svg_rectangle = R"(<rect x="{x}" y="{y}" width="{width}" height="{height}" style="fill:{{fill_color}};fill-opacity:{{fill_opacity}};stroke:none;{{svg_style}}"/>)";

const std::string svg_circle = R"(<circle cx="{x}" cy="{y}" r="{radius}" style="fill:{{fill_color}};fill-opacity:{{fill_opacity}};stroke:none;{{svg_style}}"/>)";

class NullShapeSvgSubRenderer : SVG_SHAPE_RENDERER(NullShapeSvgSubRenderer, Geom::NullShape) {
public:
    std::string operator()(boost::any const& /*shape*/) const override {
        return "";
    }
};

class PathShapeSvgSubRenderer : SVG_SHAPE_RENDERER(PathShapeSvgSubRenderer, Geom::BezierKnots) {
public:
    std::string operator()(boost::any const& shape) const override {
        auto path = boost::any_cast<Geom::BezierKnots>(shape);
        return fmt::format(svg_path, "path"_a=Geom::knots_to_svg(path));
    }
};

class RectangleShapeSvgSubRenderer : SVG_SHAPE_RENDERER(RectangleShapeSvgSubRenderer, Geom::Rectangle) {
public:
    std::string operator()(boost::any const& shape) const override {
        auto rect = boost::any_cast<Geom::Rectangle>(shape);
        return fmt::format(
            svg_rectangle,
            "x"_a=rect.pos.x(),
            "y"_a=rect.pos.y(),
            "width"_a=rect.size.x(),
            "height"_a=rect.size.y()
        );
    }
};

class CircleShapeSvgSubRenderer : SVG_SHAPE_RENDERER(CircleShapeSvgSubRenderer, Geom::Circle) {
public:
    std::string operator()(boost::any const& shape) const override {
        auto circle = boost::any_cast<Geom::Circle>(shape);
        return fmt::format(
            svg_circle,
            "x"_a=circle.pos.x(),
            "y"_a=circle.pos.y(),
            "radius"_a=circle.radius
        );
    }
};

} // namespace renderers
} // namespace core
