/*  circle.cpp - circle constructing nodes
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

#include <geom_helpers/circle.h>

namespace rainynite::core::nodes {

class CirclePR :
    public NewNode<
        CirclePR,
        Geom::Circle,
        types::Only<Geom::Point>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Construct circle from center position and radius."
    )

    NODE_PROPERTIES("position", "radius")
    DEFAULT_VALUES(Geom::Point{}, 0.0)
    PROPERTY(position)
    PROPERTY(radius)

protected:
    Geom::Circle get(shared_ptr<Context> ctx) const override {
        return {
            position_value<Geom::Point>(ctx),
            radius_value<double>(ctx)
        };
    }
};

REGISTER_NODE(CirclePR);

} // namespace rainynite::core::nodes
