/*  knot.cpp - knot <-> points conversion
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

#include <geom_helpers/knots.h>

namespace rainynite::core::nodes {

class Knot :
    public NewNode<
        Knot,
        Geom::Knot,
        types::Only<Geom::Point>,
        types::Only<Geom::Point>,
        types::Only<Geom::Point>
    >
{
    DOC_STRING(
        "Construct Bezier \"knot\""
    )

    NODE_PROPERTIES("pos", "tg1", "tg2")
    DEFAULT_VALUES(Geom::Point{}, Geom::Point{}, Geom::Point{})
    PROPERTY(pos)
    PROPERTY(tg1)
    PROPERTY(tg2)

protected:
    Geom::Knot get(shared_ptr<Context> ctx) const override {
        return Geom::Knot {
            pos_value<Geom::Point>(ctx),
            tg1_value<Geom::Point>(ctx),
            tg2_value<Geom::Point>(ctx)
        };
    }
};

REGISTER_NODE(Knot);

} // namespace rainynite::core::nodes
