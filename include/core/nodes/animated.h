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

class AbstractAnimated {
public:
    virtual void add_child(TimePeriod period, AbstractReference ref) = 0;
    virtual size_t child_count() const = 0;
};

template <class T>
class Animated : public AbstractAnimated, public Node<T> {
public:
    Animated() {
        this->template init_list<T>(children);
        this->template init_list<TimePeriod>(periods);
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
    virtual void add_child(TimePeriod period, AbstractReference ref) override {
        list_periods()->push_value(period);
        list_children()->push_back(std::dynamic_pointer_cast<BaseValue<T>>(ref));
    }
    virtual size_t child_count() const override {
        return list_children()->link_count();
    }
private:
    std::pair<AbstractReference, Time> find_appropriate(Time time) const {
        size_t i = 0;
        for (auto period : get_periods()->get(time)) {
            period.set_fps(time.get_fps());
            if (period.contains(time)) {
                return { list_children()->get_link(i), calculate_time(period, time) };
            }
            ++i;
        }
        return { get_default_value(), time };
    }
    Time calculate_time(TimePeriod const& period, Time time) const {
        auto a = period.get_first();
        auto b = period.get_last();
        return (time-a)/(b-a).get_seconds();
    }

    NODE_LIST_PROPERTY(children, T);
    NODE_LIST_PROPERTY(periods, TimePeriod);
    NODE_PROPERTY(default_value, T);
};

} // namespace nodes
} // namespace core

#endif
