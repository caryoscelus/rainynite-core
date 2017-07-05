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

#include <string>

#include <fmt/format.h>

#include "svg_module.h"

namespace core {
namespace renderers {

class CompositeSvgRenderer : SVG_RENDERER_MODULE_CLASS(CompositeSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Composite");
public:
    virtual std::string operator()(AbstractNode const& node, Time time, SvgRendererSettings const& settings) const override {
        auto list_node = node.get_property("layers");
        std::string s;
        list_node->step_into_list(
            time,
            [&s, &settings](AbstractReference child, Time t) {
                s += node_to_svg(child, t, settings);
            }
        );
        return s;
    }
};

} // namespace renderers
} // namespace core
