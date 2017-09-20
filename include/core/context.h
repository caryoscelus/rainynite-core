/*
 *  context.h - Context
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

#ifndef __CORE__CONTEXT_H__50E71540
#define __CORE__CONTEXT_H__50E71540

#include <boost/signals2/signal.hpp>

#include <core/std/memory.h>
#include <core/std/any.h>
#include <core/time/time_period.h>

namespace rainynite::core {

class Document;
template <typename>
class Value;

class Context {
public:
    Context() = default;
    explicit Context(weak_ptr<Document> document_);
    Context(Context const& context_);
    Context(Context&& context_) = default;
    Context& operator=(Context const& context_) = default;
    Context& operator=(Context&& context_) = default;
    virtual ~Context() = default;
public:
    inline shared_ptr<Document> get_document() const {
        return document.lock();
    }
    inline Time get_time() const {
        return time;
    }
    inline void set_time(Time time_) {
        time = time_;
        changed_time(time);
    }
    inline void set_frames(double frames) {
        time.set_frames(frames);
        changed_time(time);
    }
    inline void set_seconds(double seconds) {
        time.set_seconds(seconds);
        changed_time(time);
    }
    inline void to_start() {
        set_time(get_period().get_first());
    }
    inline void to_end() {
        set_time(get_period().get_last());
    }
    void set_period(TimePeriod const& period);
    TimePeriod get_period() const;
    inline any get_render_settings() const {
        return render_settings;
    }
    inline any& mod_render_settings() {
        return render_settings;
    }
    inline Time::fps_type get_fps() {
        return fps;
    }
    void set_fps(Time::fps_type fps_);
public:
    boost::signals2::signal<void(Time)> changed_time;
    boost::signals2::signal<void(Time::fps_type)> changed_fps;
private:
    weak_ptr<Document> document;
    Time::fps_type fps = 1;
    Time time;
    shared_ptr<Value<TimePeriod>> time_period;
    any render_settings;
};

} // namespace rainynite::core

#endif
