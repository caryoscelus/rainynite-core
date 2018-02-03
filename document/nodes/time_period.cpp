/*  time_period.cpp - TimePeriod related nodes
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
#include <core/time/period.h>

namespace rainynite::core::nodes {

class CompositeTimePeriod :
    public NewNode<
        CompositeTimePeriod,
        TimePeriod,
        types::Only<Time>,
        types::Only<Time>
    >
{
    DOC_STRING(
        "Construct time period from starting and one-past-last point."
    )

    NODE_PROPERTIES("first", "last")
    DEFAULT_VALUES(Time{}, Time{})
    PROPERTY(first)
    PROPERTY(last)

protected:
    TimePeriod get(shared_ptr<Context> ctx) const override {
        return {first_value<Time>(ctx), last_value<Time>(ctx)};
    }
};

REGISTER_NODE(CompositeTimePeriod);


class SizedTimePeriod :
    public NewNode<
        SizedTimePeriod,
        TimePeriod,
        types::Only<Time>,
        types::Only<Time>
    >
{
    DOC_STRING(
        "Construct time period from starting point and length."
    )

    NODE_PROPERTIES("first", "length")
    DEFAULT_VALUES(Time{}, Time{})
    PROPERTY(first)
    PROPERTY(length)

protected:
    TimePeriod get(shared_ptr<Context> ctx) const override {
        auto first = first_value<Time>(ctx);
        return {first, first + length_value<Time>(ctx)};
    }
};

REGISTER_NODE(SizedTimePeriod);


} // namespace rainynite::core::nodes
