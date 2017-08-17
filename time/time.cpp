/*
 *  time.cpp - time
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

#if __cplusplus >= 201703
#include <numeric>
#else
#include <experimental/numeric>
namespace std {
using experimental::lcm;
}
#endif

#include <core/time/time_period.h>

namespace core {

bool Time::operator==(Time const& other) const {
    return seconds == other.seconds
        && frames * other.fps == other.frames * fps;
}

bool Time::operator<(Time const& other) const {
    return seconds < other.seconds
        || (seconds == other.seconds
        && frames * other.fps < other.frames * fps);
}

Time& Time::operator++() {
    set_frames(get_frames()+1);
    return *this;
}

Time& Time::operator--() {
    set_frames(get_frames()-1);
    return *this;
}

Time& Time::operator+=(Time const& other) {
    auto t = other;
    to_common_fps(t);
    set_frames(get_frames()+t.get_frames());
    return *this;
}

Time& Time::operator-=(Time const& other) {
    auto t = other;
    to_common_fps(t);
    set_frames(get_frames()-t.get_frames());
    return *this;
}

Time& Time::operator*=(double other) {
    set_frames(get_frames()*other);
    return *this;
}

Time& Time::operator/=(double other) {
    set_frames(get_frames()/other);
    return *this;
}

double Time::operator/(Time const& other) {
    return get_seconds() / other.get_seconds();
}

void Time::require_same_fps(Time const& other) const {
    // consider zero case
    if (fps != other.fps)
        throw std::runtime_error("Time: fps mis-match");
}

Time::fps_type Time::common_fps(Time const& other) const {
    return std::lcm(get_fps(), other.get_fps());
}

void Time::to_common_fps(Time& other) {
    auto cfps = common_fps(other);
    set_fps(cfps);
    other.set_fps(cfps);
}

void Time::set_frames(double frames_) {
    auto neg = std::copysign(1, frames_);
    frames_ = std::abs(frames_);
    int whole_frames = frames_;
    seconds = neg * whole_frames / fps;
    frames = neg * (whole_frames % fps + frames_ - whole_frames);
}

void Time::add_frames(double df) {
    set_frames(get_frames()+df);
}

void Time::set_seconds(double seconds_) {
    set_frames(seconds_*fps);
}

void Time::set_fps(fps_type fps_) {
    frames = frames*fps_/fps;
    fps = fps_;
}

void Time::change_fps(fps_type fps_) {
    auto total = seconds*fps + frames;
    fps = fps_;
    set_frames(total);
}

bool TimePeriodIter::equal(iter const& other) const {
    return period == other.period
        && (now == other.now
        || before_begin() && other.before_begin()
        || after_end() && other.after_end());
}

void TimePeriodIter::advance(int diff) {
    now.add_frames(diff);
}

bool TimePeriodIter::before_begin() const {
    return now < period.get_first();
}

bool TimePeriodIter::after_end() const {
    return now >= period.get_last();
}

} // namespace core
