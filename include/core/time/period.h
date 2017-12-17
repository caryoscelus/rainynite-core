/*  time/period.h - TimePeriod
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

#ifndef CORE_TIME_PERIOD_H_AC1EAA14_64D5_5090_84BB_2AFFF35C7565
#define CORE_TIME_PERIOD_H_AC1EAA14_64D5_5090_84BB_2AFFF35C7565

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

/**
 * Period of time.
 *
 * Currently it is semi-open period: [first; last) in mathematical notation.
 *
 * Can be iterated over using for-loop.
 */
class TimePeriod : boost::equality_comparable<TimePeriod> {
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
    bool contains(Time t) const {
        return t >= first
            && t < last;
    }
    Time get_first() const {
        return first;
    }
    Time get_last() const {
        return last;
    }
    Time length() const {
        return last-first;
    }
    void set_first(Time t) {
        first = t;
    }
    void set_last(Time t) {
        last = t;
    }
    Time::fps_type get_fps() const {
        return first.get_fps();
    }
    void set_fps(Time::fps_type fps_) {
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
