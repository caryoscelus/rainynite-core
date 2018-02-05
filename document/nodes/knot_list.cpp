/*  knot_list.cpp - knot list <-> path conversion
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
#include <core/node/list.h>

#include <geom_helpers/knots.h>

namespace rainynite::core::nodes {

class KnotList :
    public NewNode<
        KnotList,
        vector<Geom::Knot>,
        types::Only<Geom::BezierKnots>
    >
{
    DOC_STRING(
        "?????"
    )

    NODE_PROPERTIES("path")
    DEFAULT_VALUES(Geom::BezierKnots{})
    PROPERTY(path)

protected:
    vector<Geom::Knot> get(shared_ptr<Context> ctx) const override {
        auto path = path_value<Geom::BezierKnots>(ctx);
        return path.knots;
    }
};

REGISTER_NODE(KnotList);


class KnotsPath :
    public NewNode<
        KnotsPath,
        Geom::BezierKnots,
        types::Only<vector<Geom::Knot>>,
        types::Only<bool>
    >
{
    DOC_STRING(
        "???"
    )

    NODE_PROPERTIES("knots", "closed")
    COMPLEX_DEFAULT_VALUES(make_node<ListValue<Geom::Knot>>(), make_value<bool>(false))
    PROPERTY(knots)
    PROPERTY(closed)

protected:
    Geom::BezierKnots get(shared_ptr<Context> ctx) const override {
        auto knots = knots_value<vector<Geom::Knot>>(ctx);
        auto closed = closed_value<bool>(ctx);
        return Geom::BezierKnots(knots, closed);
    }
};

REGISTER_NODE(KnotsPath);


} // namespace rainynite::core::nodes
