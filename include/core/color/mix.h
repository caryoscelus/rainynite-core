/*  mix.h - Color mixing
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

#ifndef CORE_COLOR_MIX_H_09269DB6_F3CE_5279_A682_E8E5238DE29F
#define CORE_COLOR_MIX_H_09269DB6_F3CE_5279_A682_E8E5238DE29F

#include "color.h"

namespace rainynite::core::colors {

template <typename T>
RGBA<T> mix(RGBA<T> const& a, RGBA<T> const& b, double amount, ConversionStyle style=ConversionStyle::Default) {
    auto ax = 1.0-amount;
    auto bx = amount;
    return RGBA<T>::from_rgba(
        a.red()*ax+b.red()*bx,
        a.green()*ax+b.green()*bx,
        a.blue()*ax+b.blue()*bx,
        a.alpha()*ax+b.alpha()*bx,
        style
    );
}

} // namespace rainynite::core::colors

#endif
