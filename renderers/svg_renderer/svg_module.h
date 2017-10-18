/*  svg_renderer/svg_module.h - SvgRenderer module class
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

#ifndef SVG_MODULE_H_F8EAFC94_D988_533E_B534_D3C49F9B27D0
#define SVG_MODULE_H_F8EAFC94_D988_533E_B534_D3C49F9B27D0

#include <core/class_init.h>
#include <core/node/abstract_node.h>
#include <core/node/abstract_value.h>
#include <core/time/time.h>

#include <2geom/point.h>

namespace rainynite::core::renderers {

struct SvgRendererSettings;

/**
 * Get "extra svg style".
 *
 * Returns value of _svg_style property or empty string if extra_style is
 * disabled in settings.
 */
string get_extra_style(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings);

/**
 * Convert node to svg string.
 *
 * TODO: use some xml generator instead of purely string based generation.
 */
string node_to_svg(NodeInContext nic, SvgRendererSettings const&);

/**
 * Abstract class for svg renderer modules.
 *
 * Svg renderer module defines one node type (defined by its registered name)
 * rendering - that is, converting to svg string.
 *
 * Register an svg renderer module using : SVG_RENDERER_MODULE_CLASS(Self)
 * inheritance and SVG_RENDERER_MODULE_NAME(node_name) inside class body.
 */
class SvgRendererModule {
public:
    virtual string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& settings) const = 0;
};

#define SVG_RENDERER_MODULE_CLASS(Self) \
public SvgRendererModule, \
private class_init::StringRegistered<Self, SvgRendererModule>

#define SVG_RENDERER_MODULE_NAME(name_) \
public: \
    static string name() { return name_; }

} // namespace rainynite::core::renderers

#endif
