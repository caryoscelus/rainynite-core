/*  renderer.h - Renderer interface
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

#ifndef CORE_RENDERER_H_E606EAD3_30E1_5114_9BF6_6D4FD2810CAA
#define CORE_RENDERER_H_E606EAD3_30E1_5114_9BF6_6D4FD2810CAA

#include <stdexcept>

#include <boost/signals2/signal.hpp>

#include <core/std/memory.h>
#include <core/std/string.h>
#include <core/time/time.h>

namespace rainynite::core {

class Context;

class RenderFailure : public std::runtime_error {
public:
    RenderFailure(string const& msg) :
        std::runtime_error(msg)
    {}
};

/**
 * Interface for Context/Document renderer.
 *
 * TODO: better control.
 */
class Renderer {

public:
    /// Render context
    virtual void render(Context&& context) = 0;

    /// Return true if rendering is done
    virtual bool is_finished() const = 0;

    /// Stop any rendering
    virtual void stop() = 0;

    /// Get finished frame signal that is called when a frame is rendered.
    inline boost::signals2::signal<void(Time)>& finished_frame() {
        return finished_frame_signal;
    }

private:
    boost::signals2::signal<void(Time)> finished_frame_signal;

};

} // namespace rainynite::core

#endif
