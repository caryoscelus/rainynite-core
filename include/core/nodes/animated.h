/*
 *  animated.h - node that switches between its children in time
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

#ifndef __CORE__NODES__ANIMATED_H__28E7E2B8
#define __CORE__NODES__ANIMATED_H__28E7E2B8

#include <core/node.h>
#include <core/time/time_period.h>

namespace core {
namespace nodes {

template <class T>
class Animated : public Node<T> {
public:
    Animated() {
        this->init(default_value, T());
    }
public:
    virtual T get(Time time) const override {
        AbstractReference r;
        Time t;
        std::tie(r, t) = find_appropriate(time);
        auto child = std::dynamic_pointer_cast<BaseValue<T>>(r);
        return child->get(t);
    }
public:
    void add_child(TimePeriod period, BaseReference<T> ref) {
        children.emplace_back(period, ref);
    }
private:
    std::pair<AbstractReference, Time> find_appropriate(Time time) const {
        for (auto const& child : children) {
            auto period = child.first;
            period.set_fps(time.get_fps());
            if (period.contains(time)) {
                return { child.second, calculate_time(period, time) };
            }
        }
        return { get_default_value(), time };
    }
    Time calculate_time(TimePeriod const& period, Time time) const {
        auto a = period.get_first();
        auto b = period.get_last();
        return (time-a)/(b-a).get_seconds();
    }
private:
    std::vector<std::pair<TimePeriod,AbstractReference>> children;

    NODE_PROPERTY(default_value, T);
};

} // namespace nodes
} // namespace core

#endif
