/*
 *  circle.cpp - circle constructing nodes
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

#include <geom_helpers/circle.h>

namespace core {
namespace nodes {

class CirclePR : public Node<Geom::Circle> {
public:
    CirclePR() {
        init<Geom::Point>(position, {});
        init<double>(radius, {});
    }
public:
    Geom::Circle get(std::shared_ptr<Context> ctx) const override {
        try {
            return {get_position()->get(ctx), get_radius()->get(ctx)};
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(position, Geom::Point);
    NODE_PROPERTY(radius, double);
};

REGISTER_NODE(CirclePR);

} // namespace nodes
} // namespace core
