/*  context.h - Context
 *  Copyright (C) 2017-2018 caryoscelus
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

#ifndef CORE_CONTEXT_H_1B1FD015_F94D_5409_AC92_8B1D26396EFA
#define CORE_CONTEXT_H_1B1FD015_F94D_5409_AC92_8B1D26396EFA

#include <boost/signals2/signal.hpp>

#include <core/std/memory.h>
#include <core/std/any.h>
#include <core/time/period.h>

namespace rainynite::core {

class AbstractDocument;
class AbstractNode;

template <typename>
class BaseValue;

/**
 * Context for node calculation and rendering.
 *
 * For now it mostly contains document, time, fps and time period.
 */
class Context {
public:
    Context() = default;
    explicit Context(weak_ptr<AbstractDocument> document_);
    Context(Context const& context_);
    Context(Context&& context_) = default;
    Context& operator=(Context const& context_) = default;
    Context& operator=(Context&& context_) = default;

    shared_ptr<AbstractDocument> get_document() const {
        return document.lock();
    }
    shared_ptr<AbstractNode> get_document_node() const;

    Time get_time() const {
        return time;
    }
    void set_time(Time time_);
    void set_frames(double frames);
    void set_seconds(double seconds);
    void to_start() {
        set_time(get_period().get_first());
    }
    void to_end() {
        set_time(get_period().get_last());
    }

    void set_period(TimePeriod const& period);
    TimePeriod get_period() const;

    Time::fps_type get_fps() {
        return fps;
    }
    void set_fps(Time::fps_type fps_);

    any get_render_settings() const {
        return render_settings;
    }
    void set_render_settings(any settings) {
        render_settings = std::move(settings);
    }

public:
    boost::signals2::signal<void(Time)> changed_time;
    boost::signals2::signal<void(Time::fps_type)> changed_fps;

private:
    weak_ptr<AbstractDocument> document;
    Time::fps_type fps = 1;
    Time time;
    shared_ptr<BaseValue<TimePeriod>> time_period;
    any render_settings;
};

} // namespace rainynite::core

#endif
