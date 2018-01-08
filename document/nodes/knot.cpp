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
#include <core/node/node.h>
#include <core/node/property.h>

#include <geom_helpers/knots.h>

namespace rainynite::core::nodes {

class Knot : public Node<Geom::Knot> {
public:
    Knot() {
        init<Geom::Point>(pos, {});
        init<Geom::Point>(tg1, {});
        init<Geom::Point>(tg2, {});
    }
protected:
    Geom::Knot get(shared_ptr<Context> ctx) const override {
        return Geom::Knot {
            get_pos()->value(ctx),
            get_tg1()->value(ctx),
            get_tg2()->value(ctx)
        };
    }

private:
    NODE_PROPERTY(pos, Geom::Point);
    NODE_PROPERTY(tg1, Geom::Point);
    NODE_PROPERTY(tg2, Geom::Point);
};

REGISTER_NODE(Knot);

} // namespace rainynite::core::nodes
