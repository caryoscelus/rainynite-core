/*
 *  image.cpp - SvgRenderer image renderer
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

namespace core {
namespace renderers {

const std::string svg_image = R"(<image xlink:href="{}" width="{}" height="{}" x="0" y="0" preserveAspectRatio="none"/>)";

class ImageSvgRenderer : SVG_RENDERER_MODULE_CLASS(ImageSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Image");
public:
    std::string operator()(AbstractNode const& node, std::shared_ptr<Context> ctx, SvgRendererSettings const& /*settings*/) const override {
        auto fname = node.get_property_as<std::string>("file_path")->get(ctx);
        auto size = node.get_property_as<Geom::Point>("size")->get(ctx);
        return fmt::format(svg_image, fname, size.x(), size.y());
    }
};

} // namespace renderers
} // namespace core
