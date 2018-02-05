/*  morph.cpp - bezier morph node
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

#include <morphing/morphing.h>

namespace rainynite::core::nodes {

class BezierMorph :
    public NewNode<
        BezierMorph,
        Geom::BezierKnots,
        types::Only<Geom::BezierKnots>,
        types::Only<Geom::BezierKnots>,
        types::Only<double>
    >
{
    DOC_STRING(
        ""
    )

    NODE_PROPERTIES("a", "b", "progress")
    DEFAULT_VALUES(Geom::BezierKnots{}, Geom::BezierKnots{}, 0.0)
    PROPERTY(a)
    PROPERTY(b)
    PROPERTY(progress)

protected:
    Geom::BezierKnots get(shared_ptr<Context> ctx) const override {
        auto a = a_value<Geom::BezierKnots>(ctx);
        auto b = b_value<Geom::BezierKnots>(ctx);
        if (a != cached_a || b != cached_b) {
            morphing::prepare_average(a, b, avg_a, avg_b);
            cached_a = a;
            cached_b = b;
        }
        auto t = progress_value<double>(ctx);
        return morphing::simple_average(avg_a, avg_b, t);
    }

private:
    mutable Geom::BezierKnots cached_a;
    mutable Geom::BezierKnots cached_b;
    mutable Geom::BezierKnots avg_a;
    mutable Geom::BezierKnots avg_b;
};

REGISTER_NODE(BezierMorph);

} // namespace rainynite::core::nodes
