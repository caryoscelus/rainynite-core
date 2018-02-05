/*  extract_coord.cpp - extract x or y coord from Point
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

#include <2geom/point.h>

namespace rainynite::core::nodes {

using Geom::X;
using Geom::Y;

template<Geom::Dim2 dimension>
class ExtractCoord :
    public NewNode<
        ExtractCoord<dimension>,
        double,
        types::Only<Geom::Point>
    >
{
    DOC_STRING(
        "Extract X or Y coordinate component from Point."
    )

    NODE_PROPERTIES("point")
    DEFAULT_VALUES(Geom::Point{})
    PROPERTY(point)

    double get(shared_ptr<Context> ctx) const override {
        auto p = point_value<Geom::Point>(ctx);
        return p[dimension];
    }
};

REGISTER_NODE_NAMED(ExtractCoord<X>, ExtractCoordXNodeInfo, "ExtractCoord/X");
REGISTER_NODE_NAMED(ExtractCoord<Y>, ExtractCoordYNodeInfo, "ExtractCoord/Y");

} // namespace rainynite::core::nodes
