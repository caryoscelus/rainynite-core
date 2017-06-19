/*
 *  time.h - Time representation
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

#ifndef __CORE__TIME__TIME_H__B4FED896
#define __CORE__TIME__TIME_H__B4FED896

#include <cmath>
#include <stdexcept>

#include <boost/operators.hpp>

namespace core {

class Time :
    boost::totally_ordered<Time>,
    boost::additive<Time>,
    boost::multiplicative<Time,double>
{
public:
    using fps_type = unsigned;
public:
    Time() :
        Time(0)
    {}
    explicit Time(int seconds_, fps_type fps_, double frames_=0.0) :
        fps(fps_)
    {
        set_frames(seconds_*fps_+frames_);
    }
    explicit Time(double seconds_) :
        fps(1)
    {
        set_frames(seconds_);
    }
    Time(Time const& other) = default;
    Time(Time&& other) = default;
    Time& operator=(Time const& other) = default;
    Time& operator=(Time&& other) = default;
public:
    bool operator==(Time const& other) const;
    bool operator<(Time const& other) const;
    Time& operator++();
    Time& operator--();
    Time& operator+=(Time const& other);
    Time& operator-=(Time const& other);
    Time& operator*=(double other);
    Time& operator/=(double other);
public:
    void require_same_fps(Time const& other) const;
public:
    double get_seconds() const {
        return seconds+frames/fps;
    }
    inline int whole_seconds() const {
        return seconds;
    }
    double get_frames() const {
        return 1.0*seconds*fps+frames;
    }
    fps_type get_fps() const {
        return fps;
    }
public:
    void set_frames(double frames_);
    void add_frames(double df);
    void set_seconds(double seconds_);
    /**
     * Change fps, preserving time in seconds
     */
    void set_fps(fps_type fps_);
    /**
     * Change fps, preserving frame count
     */
    void change_fps(fps_type fps_);
private:
    int seconds;
    double frames;
    fps_type fps;
};

} // namespace core

#endif
