/*
 *  null_shape.h - Empty Shape
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

#ifndef __CORE__NULL_SHAPE_H__97C40B4C
#define __CORE__NULL_SHAPE_H__97C40B4C

#include <ostream>

namespace Geom {

struct NullShape {};

inline std::ostream& operator<<(std::ostream& stream, NullShape) {
    return stream;
}

} // namespace Geom

#endif
