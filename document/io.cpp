/*  document/io.cpp - various document-related io impl
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

#include <core/node_tree/io.h>
#include <core/bezier_outline.h>

namespace rainynite::core {

std::ostream& operator<<(std::ostream& out, NodeTreePath const& path) {
    return out << "FAIL";
}

std::ostream& operator<<(std::ostream& out, BezierOutlinePath const& outline) {
    return out << "FAIL";
}

} // namespace rainynite::core
