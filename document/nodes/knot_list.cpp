/*
 *  knot_list.cpp - knot list <-> path conversion
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

namespace rainynite::core {
namespace nodes {

class KnotList : public Node<vector<Geom::Knot>> {
public:
    KnotList() {
        init<Geom::BezierKnots>(path, {});
    }
public:
    vector<Geom::Knot> get(shared_ptr<Context> ctx) const override {
        try {
            auto path = get_path()->get(ctx);
            return path.knots;
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(path, Geom::BezierKnots);
};

REGISTER_NODE(KnotList);

class KnotsPath : public Node<Geom::BezierKnots> {
public:
    KnotsPath() {
        init_list<Geom::Knot>(knots);
        init<bool>(closed, false);
    }
public:
    Geom::BezierKnots get(shared_ptr<Context> ctx) const override {
        try {
            auto knots = get_knots()->get(ctx);
            auto closed = get_closed()->get(ctx);
            return Geom::BezierKnots(knots, closed);
        } catch (...) {
            return {};
        }
    }

private:
    NODE_LIST_PROPERTY(knots, Geom::Knot);
    NODE_PROPERTY(closed, bool);
};

REGISTER_NODE(KnotsPath);

} // namespace nodes
} // namespace rainynite::core
