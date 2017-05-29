/*
 *  truncate.cpp - truncate floating part
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

namespace core {
namespace nodes {

class Truncate : public Node<double> {
public:
    Truncate() {
        init<double>(source, 0);
    }
public:
    virtual double get(Time time) const override {
        try {
            auto v = get_source()->get(time);
            double r;
            std::modf(v, &r);
            return r;
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(source, double);
};

REGISTER_NODE(Truncate);

} // namespace nodes
} // namespace core
