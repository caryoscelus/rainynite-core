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

} // namespace rainynite::core::renderers
