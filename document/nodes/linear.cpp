/*
 *  linear.cpp - seconds to real conversion
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

class Linear : public Node<double> {
public:
    Linear() {
        init<double>(base, 0);
        init<double>(speed, 1);
    }
public:
    virtual double get(Time time) const override {
        try {
            auto b = get_base()->get(time);
            auto f = get_speed()->get(time);
            return b+f*time.get_seconds();
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(base, double);
    NODE_PROPERTY(speed, double);
};

REGISTER_NODE(Linear);

} // namespace nodes
} // namespace core
