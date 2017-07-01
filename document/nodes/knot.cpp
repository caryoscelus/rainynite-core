/*
 *  knot.cpp - knot <-> points conversion
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

namespace core {
namespace nodes {

class Knot : public Node<Geom::Knot> {
public:
    Knot() {
        init<Geom::Point>(pos, {});
        init<Geom::Point>(tg1, {});
        init<Geom::Point>(tg2, {});
    }
public:
    virtual Geom::Knot get(Time time) const override {
        try {
            return Geom::Knot {
                get_pos()->get(time),
                get_tg1()->get(time),
                get_tg2()->get(time)
            };
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(pos, Geom::Point);
    NODE_PROPERTY(tg1, Geom::Point);
    NODE_PROPERTY(tg2, Geom::Point);
};

REGISTER_NODE(Knot);

} // namespace nodes
} // namespace core
