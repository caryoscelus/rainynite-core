/*
 *  add.cpp - add two Real numbers - mostly for testing
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

namespace core {
namespace nodes {

class Add : public Node<double> {
public:
    Add() {
        init<double>(a, 0);
        init<double>(b, 0);
    }
public:
    virtual double get(Time time) const override {
        try {
            return get_a()->get(time) + get_b()->get(time);
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(a, double);
    NODE_PROPERTY(b, double);
};

REGISTER_NODE(Add);

} // namespace nodes
} // namespace core
