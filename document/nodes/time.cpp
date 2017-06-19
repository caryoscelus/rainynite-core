/*
 *  time.cpp - time-related nodes
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

#include <core/all_types.h>
#include <core/node_info.h>
#include <core/node/proxy_node.h>
#include <core/node/property.h>

namespace core {
namespace nodes {

template <typename T>
class TimeMap : public ProxyNode<T> {
public:
    TimeMap() {
        this->template init<T>(source, {});
        this->template init<double>(multiplier, 1);
        this->template init<Time>(offset, {});
    }
public:
    virtual void step_into(Time time, std::function<void(AbstractReference,Time)> f) const override {
        auto t = time * get_multiplier()->get(time) + get_offset()->get(time);
        f(get_source(), t);
    }

private:
    NODE_PROPERTY(source, T);
    NODE_PROPERTY(multiplier, double);
    NODE_PROPERTY(offset, Time);
};

NODE_INFO_TEMPLATE(TimeMap, TimeMap<T>, T);
TYPE_INSTANCES(TimeMapNodeInfo)

} // namespace nodes
} // namespace core
