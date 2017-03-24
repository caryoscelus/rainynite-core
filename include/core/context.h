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

#include <memory>

#include <boost/any.hpp>
#include <boost/signals2/signal.hpp>

#include <core/time/time_period.h>

namespace core {

class Document;

class Context {
public:
    explicit Context() = default;
    explicit Context(std::weak_ptr<Document> document_);
    Context(Context const& context_);
    Context(Context&& context_) = default;
    Context& operator=(Context const& context_) = default;
    Context& operator=(Context&& context_) = default;
    virtual ~Context();
public:
    inline std::shared_ptr<Document> get_document() const {
        return document.lock();
    }
    inline Time get_time() const {
        return time;
    }
    inline void set_time(Time time_) {
        time = time_;
        changed_time(time);
    }
    inline TimePeriod get_period() const {
        return time_period;
    }
    inline boost::any get_render_settings() const {
        return render_settings;
    }
    inline boost::any& mod_render_settings() {
        return render_settings;
    }
    inline Time::fps_type get_fps() {
        return fps;
    }
    inline void set_fps(Time::fps_type fps_) {
        fps = fps_;
        time.set_fps(fps);
        time_period.set_fps(fps);
    }
public:
    boost::signals2::signal<void(Time)> changed_time;
private:
    std::weak_ptr<Document> document;
    Time::fps_type fps = 1;
    Time time;
    TimePeriod time_period;
    boost::any render_settings;
};

} // namespace core

#endif
