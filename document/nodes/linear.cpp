/*  linear.cpp - seconds to real conversion
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
#include <core/context.h>

namespace rainynite::core::nodes {

class Linear : public Node<double> {
public:
    Linear() {
        init<double>(base, 0);
        init<double>(speed, 1);
    }
protected:
    double get(shared_ptr<Context> ctx) const override {
        auto b = get_base()->value(ctx);
        auto f = get_speed()->value(ctx);
        return b+f*ctx->get_time().get_seconds();
    }

private:
    NODE_PROPERTY(base, double);
    NODE_PROPERTY(speed, double);
};

REGISTER_NODE(Linear);

} // namespace rainynite::core::nodes
