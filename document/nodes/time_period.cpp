/*
 *  time_period.cpp - TimePeriod related nodes
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/time/time_period.h>

namespace rainynite::core {
namespace nodes {

class CompositeTimePeriod : public Node<TimePeriod> {
public:
    CompositeTimePeriod() {
        init<Time>(first, {});
        init<Time>(last, {});
    }
public:
    TimePeriod get(shared_ptr<Context> ctx) const override {
        return {get_first()->get(ctx), get_last()->get(ctx)};
    }

private:
    NODE_PROPERTY(first, Time);
    NODE_PROPERTY(last, Time);
};

REGISTER_NODE(CompositeTimePeriod);

class SizedTimePeriod : public Node<TimePeriod> {
public:
    SizedTimePeriod() {
        init<Time>(first, {});
        init<Time>(length, {});
    }
public:
    TimePeriod get(shared_ptr<Context> ctx) const override {
        auto first = get_first()->get(ctx);
        return {first, first+get_length()->get(ctx)};
    }

private:
    NODE_PROPERTY(first, Time);
    NODE_PROPERTY(length, Time);
};

REGISTER_NODE(SizedTimePeriod);

} // namespace nodes
} // namespace rainynite::core
