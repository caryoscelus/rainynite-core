/*  notify.h - abstract class for things that notify about their changes
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

#ifndef CORE_NODE_NOTIFY_H_BFB9C4AC_D9A9_552F_B76B_454E86DB73E8
#define CORE_NODE_NOTIFY_H_BFB9C4AC_D9A9_552F_B76B_454E86DB73E8

#include <boost/signals2/signal.hpp>

#include <core/std/memory.h>
#include <core/destroy_detector.h>

namespace rainynite::core {

/**
 * Abstract entity that notifies of its changes.
 *
 * TODO: move out of nodes
 */
class AbstractNotify : public DestroyDetector {
public:
    AbstractNotify() :
        DestroyDetector(),
        changed_signal()
    {}
    AbstractNotify(AbstractNotify const& /*other*/) :
        DestroyDetector(),
        changed_signal()
    {}
    virtual ~AbstractNotify() = default;

    /**
     * Function to be called when object has changed.
     */
    inline void changed() {
        changed_signal();
    }

    /**
     * Subscribe to this object changes
     */
    template <typename F>
    boost::signals2::connection subscribe(F f) {
        return connect_boost(changed_signal, f);
    }

private:
    /**
     * Signal to subscribe to this object's changes
     */
    boost::signals2::signal<void()> changed_signal;
};

} // namespace rainynite::core

#endif
