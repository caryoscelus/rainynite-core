/*
 *  transform.cpp - SvgRenderer transform renderer
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

#include "svg_module.h"

using namespace fmt::literals;

namespace core {
namespace renderers {

const std::string svg_translate = R"svg(<g transform="translate({}, {})">{}</g>)svg";
const std::string svg_scale = R"svg(<g transform="scale({x}, {y})">{source}</g>)svg";
const std::string svg_rotate = R"svg(<g transform="rotate({angle}, {x}, {y})">{source}</g>)svg";

class TranslateSvgRenderer : SVG_RENDERER_MODULE_CLASS(TranslateSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Translate");
public:
    std::string operator()(AbstractNode const& node, std::shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto source = node.get_property("source");
        auto offset = node.get_property_as<Geom::Point>("offset")->get(ctx);
        return fmt::format(svg_translate, offset.x(), offset.y(), node_to_svg({source, ctx}, settings));
    }
};

class ScaleSvgRenderer : SVG_RENDERER_MODULE_CLASS(ScaleSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Scale");
public:
    std::string operator()(AbstractNode const& node, std::shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
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
    std::string operator()(AbstractNode const& node, std::shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
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

} // namespace renderers
} // namespace core
