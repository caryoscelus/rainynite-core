/*  shapes.h - Shape type constraint
 *  Copyright (C) 2018 caryoscelus
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

#ifndef CORES_SHAPES_H_8C303A77_6D0B_5537_9F83_E0393C42C6E3
#define CORES_SHAPES_H_8C303A77_6D0B_5537_9F83_E0393C42C6E3

#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/circle.h>
#include <geom_helpers/knots.h>

namespace rainynite::core::types {

using Shape = AnyOf<
    Geom::NullShape,
    Geom::Rectangle,
    Geom::Circle,
    Geom::BezierKnots
>;

} // namespace rainynite::core::types

#endif
