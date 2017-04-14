/*
 *  animated.cpp - node that switches between its children in time
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

#include <geom_helpers/knots.h>

#include <core/node_info.h>
#include <core/nodes/animated.h>
#include <core/types.h>

namespace core {
namespace nodes {

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

template <class T>
class AnimatedNodeInfo :
    public NodeInfo,
    class_init::Registered<AnimatedNodeInfo<T>, Animated<T>, NodeInfo>,
    class_init::ReverseRegistered<AnimatedNodeInfo<T>, Animated<T>, std::string>
{
public:
    virtual std::string operator()() const override {
        return "Animated<"+class_init::type_info<TypeInfo,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return std::make_shared<Animated<T>>();
    }
};

template class AnimatedNodeInfo<Geom::BezierKnots>;

} // namespace nodes
} // namespace core