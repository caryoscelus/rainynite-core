/*
 *  morph.cpp - bezier morph node
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

#include <geom_helpers/knots.h>

#include <morphing/morphing.h>

namespace core {
namespace nodes {

class BezierMorph : public Node<Geom::BezierKnots> {
public:
    BezierMorph() {
        init<Geom::BezierKnots>(a, {});
        init<Geom::BezierKnots>(b, {});
        init<double>(progress, 0);
    }
public:
    Geom::BezierKnots get(Time time) const override {
        try {
            auto a = get_a()->get(time);
            auto b = get_b()->get(time);
            if (a != cached_a || b != cached_b) {
                morphing::prepare_average(a, b, avg_a, avg_b);
                cached_a = a;
                cached_b = b;
            }
            auto t = get_progress()->get(time);
            return morphing::simple_average(avg_a, avg_b, t);
        } catch (...) {
            // TODO: DEBUG
            return Geom::BezierKnots();
        }
    }

private:
    mutable Geom::BezierKnots cached_a;
    mutable Geom::BezierKnots cached_b;
    mutable Geom::BezierKnots avg_a;
    mutable Geom::BezierKnots avg_b;

    NODE_PROPERTY(a, Geom::BezierKnots);
    NODE_PROPERTY(b, Geom::BezierKnots);
    NODE_PROPERTY(progress, double);
};

REGISTER_NODE(BezierMorph);

} // namespace nodes
} // namespace core
