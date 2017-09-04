/*
 *  time_period.h - TimePeriod
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

#ifndef __CORE__TIME__TIME_PERIOD_H__4CC9C668
#define __CORE__TIME__TIME_PERIOD_H__4CC9C668

#include <boost/iterator/iterator_facade.hpp>

#include "time.h"

namespace rainynite::core {

class TimePeriod;

class TimePeriodIter : public boost::iterator_facade<
    TimePeriodIter,
    Time,
    std::random_access_iterator_tag,
    Time
> {
public:
    using iter = TimePeriodIter;
    using value = Time;
public:
    explicit TimePeriodIter(TimePeriod const& period_, Time now_) :
        period(period_),
        now(now_)
    {}
public:
    value dereference() const {
        return now;
    }
    bool equal(iter const& other) const;
    void advance(int diff);
    void increment() {
        advance(1);
    }
    void decrement() {
        advance(-1);
    }
//     int distance_to(iter const& other) const
private:
    bool before_begin() const;
    bool after_end() const;
private:
    TimePeriod const& period;
    Time now;
};

class TimePeriod : boost::operators<TimePeriod> {
public:
    TimePeriod() :
        first(0, 1), last(0, 1), fps(1)
    {}
    TimePeriod(Time first_, Time last_) :
        first(first_), last(last_)
    {
        first.to_common_fps(last);
        fps = first.get_fps();
    }
    TimePeriod(TimePeriod const& other) :
        first(other.first), last(other.last), fps(other.fps)
    {}
public:
    bool operator==(TimePeriod const& other) const {
        return first == other.first
            && last == other.last
            && fps == other.fps;
    }
public:
    TimePeriodIter begin() const {
        return TimePeriodIter(*this, first);
    }
    TimePeriodIter end() const {
        return TimePeriodIter(*this, last);
    }
public:
    inline bool contains(Time t) const {
        return t >= first
            && t < last;
    }
    inline Time get_first() const {
        return first;
    }
    inline Time get_last() const {
        return last;
    }
    inline void set_first(Time t) {
        first = t;
    }
    inline void set_last(Time t) {
        last = t;
    }
    inline Time::fps_type get_fps() const {
        return first.get_fps();
    }
    inline void set_fps(Time::fps_type fps_) {
        fps = fps_;
        first.set_fps(fps);
        last.set_fps(fps);
    }
private:
    Time first;
    Time last;
    Time::fps_type fps;
};

} // namespace rainynite::core

#endif
