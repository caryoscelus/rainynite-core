/*  time.h - Time representation
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

#ifndef CORE_TIME_TIME_H_46A52DEB_6768_5920_B301_D60EE5322217
#define CORE_TIME_TIME_H_46A52DEB_6768_5920_B301_D60EE5322217

#include <cmath>
#include <stdexcept>
#include <limits>

#include <boost/operators.hpp>

namespace rainynite::core {

/**
 * Time type.
 *
 * TODO: consider storing only frame count; or otherwise store frames as fixed
 * point number.
 */
class Time :
    boost::totally_ordered<Time>,
    boost::additive<Time>,
    boost::multiplicative<Time,double>
{
public:
    using fps_type = unsigned short;
public:
    Time() :
        Time(0)
    {}
    explicit Time(double seconds_, fps_type fps_, double frames_=0.0) :
        fps(fps_)
    {
        set_frames(seconds_*fps_+frames_);
    }
    explicit Time(double seconds_) :
        fps(1)
    {
        set_frames(seconds_);
    }
    static Time start_of_time() {
        return Time(-std::numeric_limits<double>::infinity());
    }
    static Time end_of_time() {
        return Time(std::numeric_limits<double>::infinity());
    }

    bool operator==(Time const& other) const;
    bool operator<(Time const& other) const;
    Time& operator++();
    Time& operator--();
    Time& operator+=(Time const& other);
    Time& operator-=(Time const& other);
    Time& operator*=(double other);
    Time& operator/=(double other);
    double operator/(Time const& other);

    void require_same_fps(Time const& other) const;
    fps_type common_fps(Time const& other) const;
    void to_common_fps(Time& other);

    /// Get total second count
    double get_seconds() const {
        return seconds+frames/fps;
    }

    /// Get amount of whole seconds
    int whole_seconds() const {
        return seconds;
    }

    /// Get total frame count
    double get_frames() const {
        return 1.0*seconds*fps+frames;
    }

    /// Get amount of frame reminder
    double only_frames() const {
        return frames;
    }

    fps_type get_fps() const {
        return fps;
    }

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

} // namespace rainynite::core

#endif
