/*  transform.cpp - SvgRenderer transform renderer
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

#include <2geom/affine.h>

#include <core/serialize/node_writer.h>
#include "svg_module.h"

using namespace fmt::literals;

namespace rainynite::core::renderers {

const string svg_translate = R"svg(<g transform="translate({}, {})">{}</g>)svg";
const string svg_scale = R"svg(<g transform="scale({x}, {y})">{source}</g>)svg";
const string svg_rotate = R"svg(<g transform="rotate({angle}, {x}, {y})">{source}</g>)svg";
const string svg_matrix = R"svg(<g transform="{matrix_2x3}">{source}</g>)svg";

class TransformSvgRenderer : SVG_RENDERER_MODULE_CLASS(TransformSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Transform");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto source = node.get_property("source");
        auto matrix = node.get_property_as<Geom::Affine>("transform")->value(ctx);
        return fmt::format(
            svg_matrix,
            "matrix_2x3"_a=serialize::value_to_string(matrix),
            "source"_a=node_to_svg({source, ctx}, settings)
        );
    }
};

class TranslateSvgRenderer : SVG_RENDERER_MODULE_CLASS(TranslateSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Translate");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto source = node.get_property("source");
        auto offset = node.get_property_as<Geom::Point>("offset")->get(ctx);
        return fmt::format(svg_translate, offset.x(), offset.y(), node_to_svg({source, ctx}, settings));
    }
};

class ScaleSvgRenderer : SVG_RENDERER_MODULE_CLASS(ScaleSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Scale");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto source = node.get_property("source");
        auto scale = node.get_property_as<Geom::Point>("scale")->get(ctx);
        return fmt::format(
            svg_scale,
            "x"_a=scale.x(),
            "y"_a=scale.y(),
            "source"_a=node_to_svg({source, ctx}, settings)
        );
    }
};

class RotateSvgRenderer : SVG_RENDERER_MODULE_CLASS(RotateSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Rotate");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto source = node.get_property("source");
        auto angle = node.get_property_as<double>("angle")->get(ctx);
        auto origin = node.get_property_as<Geom::Point>("origin")->get(ctx);
        return fmt::format(
            svg_rotate,
            "angle"_a=angle*360,
            "x"_a=origin.x(),
            "y"_a=origin.y(),
            "source"_a=node_to_svg({source, ctx}, settings)
        );
    }
};

} // namespace rainynite::core::renderers
