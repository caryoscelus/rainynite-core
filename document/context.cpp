/*
 *  context.cpp - Context
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

#include <core/context.h>
#include <core/document.h>

namespace core {

Context::Context(std::weak_ptr<Document> document_) :
    document(document_)
{
    auto doc = get_document();
    if (!doc)
        return;
    time_period = doc->get_main_time_period();
    fps = get_period().get_fps();
    time = Time(0, fps);
}

Context::Context(Context const& context_) :
    Context(context_.document)
{
    render_settings = context_.render_settings;
    fps = context_.fps;
    time_period = context_.time_period;
    time = context_.time;
}

Context::~Context() {
}

TimePeriod Context::get_period() const {
    return time_period->mod();
}

void Context::set_fps(Time::fps_type fps_) {
    fps = fps_;
    time.set_fps(fps);
    // NOTE: this will not notify!
    time_period->mod().set_fps(fps);
}

} // namespace core
