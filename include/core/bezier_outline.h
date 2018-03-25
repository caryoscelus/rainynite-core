/*  bezier_outline.h - temporary (?) shape type for variable width outlines
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

#ifndef CORE_BEZIER_OUTLINE_H_C6C18303_28DC_5941_BDB3_8D4D800BF64A
#define CORE_BEZIER_OUTLINE_H_C6C18303_28DC_5941_BDB3_8D4D800BF64A

#include <iostream>

#include <core/std/vector.h>
#include <core/std/tuple.h>

#include <geom_helpers/knots.h>

namespace rainynite::core {

struct BezierOutlinePath {
    Geom::BezierKnots source;
    vector<pair<double,double>> widths;
};

std::ostream& operator<<(std::ostream& out, BezierOutlinePath const& outline);

} // namespace rainynite::core

#endif
