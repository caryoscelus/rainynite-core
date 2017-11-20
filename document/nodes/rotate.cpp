/*  rotate.cpp - Rotate transformation node
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

#include <boost/math/constants/constants.hpp>

#include <core/node/node.h>
#include <core/node/property.h>
#include <core/node_info.h>

#include <2geom/transforms.h>

using boost::math::double_constants::pi;

namespace rainynite::core::nodes {

class Rotate : public Node<Geom::Affine> {
    DOC_STRING(
        "Rotate transformation."
    )
public:
    Rotate() {
        init<Geom::Affine>(source, {});
        init<Geom::Point>(origin, {});
        init<double>(angle, {});
    }

    Geom::Affine get(shared_ptr<Context> ctx) const override {
        auto offset = get_origin()->value(ctx);
        auto turns = get_angle()->value(ctx);
        return
            get_source()->value(ctx) *
            Geom::Translate(-offset) *
            Geom::Rotate(turns * pi * 2) *
            Geom::Translate(offset);
    }

private:
    NODE_PROPERTY(source, Geom::Affine);
    NODE_PROPERTY(origin, Geom::Point);
    NODE_PROPERTY(angle, double);
};

REGISTER_NODE(Rotate);

} // namespace rainynite::core::nodes