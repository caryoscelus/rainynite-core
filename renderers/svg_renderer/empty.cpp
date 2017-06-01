/*
 *  empty.cpp - SvgRenderer Empty renderer
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

#include "svg_module.h"

namespace core {
namespace renderers {

class EmptySvgRenderer : SVG_RENDERER_MODULE_CLASS(EmptySvgRenderer) {
    SVG_RENDERER_MODULE_NAME("Empty");
public:
    virtual std::string operator()(AbstractNode const& /*node*/, Time /*time*/, SvgRendererSettings const& /*settings*/) const override {
        return "";
    }
};

} // namespace renderers
} // namespace core
