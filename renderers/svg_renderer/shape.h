/*  svg_renderer/shape.h - Svg renderer shape module definitions
 *  Copyright (C) 2017-2018 caryoscelus
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
struct SvgShapeRenderer {
    /// Render shape/shading/extra_style to svg xml string
    virtual string operator()(any const& shape, Shading const& shading, string extra_style, string extra_defs) const = 0;
};

/**
 * Simple shape renderer, suitable for most cases
 */
struct DefaultSvgShapeRenderer : public SvgShapeRenderer {
    string operator()(any const& shape, Shading const& shading, string extra_style, string extra_defs) const override;

    /// Return main svg shape tag & params part
    virtual string get_main_shape(any const& shape) const = 0;
};

/**
 * Render shape to .svg.
 *
 * Accepts shape of any type that has a registered SvgShapeRenderer for it
 */
template <typename... Args>
string render_svg_shape(any const& shape, Args&&... args) {
    return class_init::type_info<SvgShapeRenderer, string>(shape.type(), shape, std::forward<Args>(args)...);
}

#define REGISTER_SVG_SHAPE_RENDERER(Self, Shape) \
private class_init::Registered<Self, Shape, SvgShapeRenderer>

#define SVG_SHAPE_RENDERER(Self, Shape) \
public DefaultSvgShapeRenderer, \
REGISTER_SVG_SHAPE_RENDERER(Self, Shape)

} // namespace rainynite::core::renderers

#endif
