/*  shading.h - 2d shading type
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

#ifndef CORE_SHADING_H_99743AB7_6468_5F6C_8C91_1C36017729CB
#define CORE_SHADING_H_99743AB7_6468_5F6C_8C91_1C36017729CB

#include <core/color/color.h>

namespace rainynite::core {

/**
 * Generic shape shading type.
 *
 * Currently it only supports fill&line colors and implementation-defined
 * custom shading. In the future, it should support gradients, textures etc.
 */
struct Shading {
    using Color = colors::Color;
    Color fill_color;
    Color line_color;
    double line_width;
    string custom_shading;

    static Shading default_shading();
};

std::ostream& operator<<(std::ostream& stream, Shading const& shading);

} // namespace rainynite::core

#endif
