/*  svg_renderer/shape.h - Svg renderer shape module definitions
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

#ifndef SHAPE_H_21EA882C_4435_5D12_BDB2_AA2007E61909
#define SHAPE_H_21EA882C_4435_5D12_BDB2_AA2007E61909

namespace rainynite::core::renderers {

/**
 * Base class for svg shape render modules.
 *
 * Use SVG_SHAPE_RENDERER when declaring render module to automatically
 * register it.
 */
class SvgShapeRenderer {
public:
    /**
     * Render shape to svg.
     *
     * Style is provided by generic renderer, so this function should provide
     * string with places to replace: "{fill_color}", "{svg_style}" and possibly
     * more in the future.
     */
    virtual string operator()(any const& shape) const = 0;
};

/**
 * Render shape to .svg.
 *
 * Accepts shape of any type that has a registered SvgShapeRenderer for it
 */
inline string render_svg_shape(any const& shape) {
    return class_init::any_info<SvgShapeRenderer, string>(shape);
}

#define SVG_SHAPE_RENDERER(Self, Shape) \
public SvgShapeRenderer, \
private class_init::Registered<Self, Shape, SvgShapeRenderer>

} // namespace rainynite::core::renderers

#endif
