/*  math.cpp - math operations nodes
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

#include <cmath>

#include <boost/math/constants/constants.hpp>

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

using boost::math::double_constants::pi;

namespace rainynite::core::nodes {

class Multiply : public Node<double> {
public:
    Multiply() {
        init<double>(a, 0);
        init<double>(b, 0);
    }
public:
    double get(shared_ptr<Context> ctx) const override {
        try {
            return get_a()->get(ctx) * get_b()->get(ctx);
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(a, double);
    NODE_PROPERTY(b, double);
};
REGISTER_NODE(Multiply);

class Sin : public Node<double> {
public:
    Sin() {
        init<double>(turns, 0);
    }
public:
    double get(shared_ptr<Context> ctx) const override {
        try {
            return std::sin(get_turns()->get(ctx) * pi * 2);
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(turns, double);
};
REGISTER_NODE(Sin);

} // namespace rainynite::core::nodes
