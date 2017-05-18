/*
 *  rectangle.cpp - rectangle constructing nodes
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

#include <geom_helpers/rectangle.h>

namespace core {
namespace nodes {

class RectangleWH : public Node<Geom::Rectangle> {
public:
    RectangleWH() {
        init<Geom::Point>(position, {});
        init<Geom::Point>(size, {});
    }
public:
    virtual Geom::Rectangle get(Time time) const override {
        try {
            return {get_position()->get(time), get_size()->get(time)};
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(position, Geom::Point);
    NODE_PROPERTY(size, Geom::Point);
};

REGISTER_NODE(RectangleWH);


class RectangleAB : public Node<Geom::Rectangle> {
public:
    RectangleAB() {
        init<Geom::Point>(a, {});
        init<Geom::Point>(b, {});
    }
public:
    virtual Geom::Rectangle get(Time time) const override {
        try {
            return Geom::Rectangle::fromTwoPoints(get_a()->get(time), get_b()->get(time));
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(a, Geom::Point);
    NODE_PROPERTY(b, Geom::Point);
};

REGISTER_NODE(RectangleAB);

} // namespace nodes
} // namespace core