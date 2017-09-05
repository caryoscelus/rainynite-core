/*
 *  composite.cpp - SvgRenderer composite renderer
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

#include "svg_module.h"

namespace rainynite::core {
namespace renderers {

class CompositeSvgRenderer : SVG_RENDERER_MODULE_CLASS(CompositeSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Composite");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const override {
        auto list_node = node.get_property("layers");
        string s;
        list_node->step_into_list(
            ctx,
            [&s, &settings](NodeInContext e) {
                s += node_to_svg(e, settings);
            }
        );
        return s;
    }
};

} // namespace renderers
} // namespace rainynite::core
