/*
 *  svg_module.h - SvgRenderer module class
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

#ifndef __CORE__RENDERERS__SVG_RENDERER__SVG_MODULE_H__06E36F72
#define __CORE__RENDERERS__SVG_RENDERER__SVG_MODULE_H__06E36F72

#include <core/class_init.h>
#include <core/node.h>
#include <core/time/time.h>

#include <2geom/point.h>

namespace core {
namespace renderers {

struct SvgRendererSettings;

std::string get_extra_style(AbstractNode const& node, Time time, SvgRendererSettings const& settings);

std::string node_to_svg(AbstractReference node_ptr, Time time, SvgRendererSettings const&);

class SvgRendererModule {
public:
    virtual std::string operator()(AbstractNode const& node, Time time, SvgRendererSettings const& settings) const = 0;
};

#define SVG_RENDERER_MODULE_CLASS(Self) \
public SvgRendererModule, \
private class_init::StringRegistered<Self, SvgRendererModule>

#define SVG_RENDERER_MODULE_NAME(name_) \
public: \
    static std::string name() { return name_; }

} // namespace renderers
} // namespace core

#endif
