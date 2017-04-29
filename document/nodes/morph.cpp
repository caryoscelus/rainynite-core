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

#include <core/node.h>
#include <core/node_info.h>

#include <geom_helpers/knots.h>

#include <morphing/morphing.h>

namespace core {
namespace nodes {

class BezierMorph : public Node<Geom::BezierKnots> {
public:
    BezierMorph() {
        init(a, Geom::BezierKnots());
        init(b, Geom::BezierKnots());
    }
public:
    virtual Geom::BezierKnots get(Time time) const override {
        try {
            auto a = get_a()->get(time);
            auto b = get_b()->get(time);
            if (a != cached_a && b != cached_b) {
                cached_a = a;
                cached_b = b;
                morphing::prepare_average(cached_a, cached_b, avg_a, avg_b);
            }
            // TODO: configure time?
            auto t = time.get_seconds();
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
};

REGISTER_NODE(BezierMorph);

} // namespace nodes
} // namespace core
