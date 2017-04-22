/*
 *  extract_coord.cpp - extract x or y coord from Point
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

namespace core {
namespace nodes {

using Geom::X;
using Geom::Y;

template<Geom::Dim2 dimension>
class ExtractCoord : public Node<double> {
public:
    ExtractCoord() {
        init<Geom::Point>(point, {});
    }
public:
    virtual double get(Time time) const override {
        try {
            auto p = get_point()->get(time);
            return p[dimension];
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(point, Geom::Point);
};

REGISTER_NODE_NAMED(ExtractCoord<X>, ExtractCoordXNodeInfo, "ExtractCoord<X>");
REGISTER_NODE_NAMED(ExtractCoord<Y>, ExtractCoordYNodeInfo, "ExtractCoord<Y>");


} // namespace nodes
} // namespace core
