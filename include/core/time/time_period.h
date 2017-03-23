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

namespace core {

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
        first(0), last(0), spf(Time(1.0))
    {}
    TimePeriod(Time first_, Time last_, Time spf_=Time(1.0)) :
        first(first_), last(last_), spf(spf_)
    {
        if (spf <= Time(0))
            throw "incorrect spf";
    }
    TimePeriod(TimePeriod const& other) :
        first(other.first), last(other.last), spf(other.spf)
    {}
public:
    bool operator==(TimePeriod const& other) const {
        return first == other.first
            && last == other.last
            && spf == other.spf;
    }
public:
    TimePeriodIter begin() const {
        return TimePeriodIter(*this, first);
    }
    TimePeriodIter end() const {
        return TimePeriodIter(*this, last);
    }
public:
    inline Time get_spf() const {
        return spf;
    }
    inline Time get_first() const {
        return first;
    }
    inline Time get_last() const {
        return last;
    }
private:
    Time first;
    Time last;
    Time spf;
};

} // namespace core

#endif
