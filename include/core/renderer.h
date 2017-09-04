/*
 *  renderer.h - Renderer interface
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

#ifndef __CORE__RENDERER_H__67308EB8
#define __CORE__RENDERER_H__67308EB8

#include <memory>
#include <stdexcept>

#include <boost/signals2/signal.hpp>

#include <core/time/time.h>

namespace rainynite::core {

class Context;

class RenderFailure : public std::runtime_error {
public:
    RenderFailure(std::string const& msg) :
        std::runtime_error(msg)
    {}
};

class Renderer {
public:
    virtual void render(Context&& context) = 0;
    virtual bool is_finished() const = 0;
    virtual void stop() = 0;
public:
    inline boost::signals2::signal<void(Time)>& finished_frame() {
        return finished_frame_signal;
    }
private:
    boost::signals2::signal<void(Time)> finished_frame_signal;
};

} // namespace rainynite::core

#endif
