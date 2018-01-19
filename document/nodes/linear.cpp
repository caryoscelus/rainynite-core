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
#include <core/node/new_node.h>
#include <core/context.h>

namespace rainynite::core::nodes {

class Linear :
    public NewNode<
        Linear,
        double,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Linear function\n"
        "\n"
        "In other words, convert Time in seconds to Real value"
    )

    NODE_PROPERTIES("base", "speed")
    DEFAULT_VALUES(0.0, 1.0)
    PROPERTY(base)
    PROPERTY(speed)

protected:
    double get(shared_ptr<Context> ctx) const override {
        auto b = base_value<double>(ctx);
        auto f = speed_value<double>(ctx);
        return b + f*ctx->get_time().get_seconds();
    }
};

REGISTER_NODE(Linear);

} // namespace rainynite::core::nodes
