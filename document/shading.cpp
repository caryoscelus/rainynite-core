/*  shading.cpp - 2d shading type
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

#include <iostream>

#include <core/shading.h>

namespace rainynite::core {

Shading Shading::default_shading() {
    return {
        { 0xff, 0xff, 0xff, 0xff },
        { 0x00, 0x00, 0x00, 0xff },
        ""
    };
}

std::ostream& operator<<(std::ostream& stream, Shading const& /*shading*/) {
    return stream << "Shading???";
}

}
