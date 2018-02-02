/*  truncate.cpp - truncate floating part
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

#include <cmath>

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

namespace rainynite::core::nodes {

class Truncate :
    public NewNode<
        Truncate,
        double,
        types::Only<double>
    >
{
    DOC_STRING(
        "Get integer part of real (round towards 0)"
    )

    NODE_PROPERTIES("source")
    DEFAULT_VALUES(0.0)

    PROPERTY(source)

protected:
    double get(shared_ptr<Context> ctx) const override {
        auto v = source_value<double>(ctx);
        double r;
        std::modf(v, &r);
        return r;
    }
};

REGISTER_NODE(Truncate);

} // namespace rainynite::core::nodes
