/*  shapes.cpp - SvgRenderer shape renderer
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

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fmt/format.h>

#include <core/bezier_outline.h>
#include "svg_module.h"
#include "shape.h"

#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/circle.h>
#include <geom_helpers/knots.h>

using namespace fmt::literals;

namespace rainynite::core::renderers {

const string svg_path = R"(path d="{path}" inkscape:original-d="{path}")";

const string svg_powerstroke_def = R"(<inkscape:path-effect
    effect="powerstroke"
    id="{id}"
    is_visible="true"
    offset_points="{width_list}"
    sort_points="true"
    interpolator_type="SpiroInterpolator"
    interpolator_beta="0.073529412"
    start_linecap_type="butt"
    linejoin_type="extrp_arc"
    miter_limit="4"
    end_linecap_type="butt" />)";

const string svg_rectangle = R"(rect x="{x}" y="{y}" width="{width}" height="{height}")";

const string svg_circle = R"(circle cx="{x}" cy="{y}" r="{radius}")";

class NullShapeSvgSubRenderer : SVG_SHAPE_RENDERER(NullShapeSvgSubRenderer, Geom::NullShape) {
public:
    string get_main_shape(any const& /*shape*/) const override {
        return "";
    }
};

class PathShapeSvgSubRenderer : SVG_SHAPE_RENDERER(PathShapeSvgSubRenderer, Geom::BezierKnots) {
public:
    string get_main_shape(any const& shape) const override {
        auto path = any_cast<Geom::BezierKnots>(shape);
        return fmt::format(svg_path, "path"_a=Geom::knots_to_svg(path));
    }
};

class RectangleShapeSvgSubRenderer : SVG_SHAPE_RENDERER(RectangleShapeSvgSubRenderer, Geom::Rectangle) {
public:
    string get_main_shape(any const& shape) const override {
        auto rect = any_cast<Geom::Rectangle>(shape);
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
    string get_main_shape(any const& shape) const override {
        auto circle = any_cast<Geom::Circle>(shape);
        return fmt::format(
            svg_circle,
            "x"_a=circle.pos.x(),
            "y"_a=circle.pos.y(),
            "radius"_a=circle.radius
        );
    }
};

class VariableWidthOutlineSvgSubRenderer :
    public SvgShapeRenderer,
    REGISTER_SVG_SHAPE_RENDERER(VariableWidthOutlineSvgSubRenderer, BezierOutlinePath) {
public:
    string operator()(any const& shape, Shading const& shading, string extra_style, string extra_defs) const override {
        auto powerstroke_id = to_string(boost::uuids::random_generator()());
        auto outline = any_cast<BezierOutlinePath>(shape);
        extra_defs += fmt::format(R"( inkscape:path-effect="#{}")", powerstroke_id);

        string widths_str;
        // TODO
        for (auto const& e : outline.widths) {
            if (widths_str != "")
                widths_str += " | ";
            widths_str += "{},{}"_format(e.first, e.second);
        }

        auto powerstroke = fmt::format(
            svg_powerstroke_def,
            "id"_a=powerstroke_id,
            "width_list"_a=widths_str
        );

        // TODO
        auto main = PathShapeSvgSubRenderer()(outline.source, shading, extra_style, extra_defs);
        return "{powerstroke}{shape}"_format(
            "powerstroke"_a=powerstroke,
            "shape"_a=main
        );
    }
};

} // namespace rainynite::core::renderers
