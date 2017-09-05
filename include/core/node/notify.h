/*
 *  notify.h - abstract class for things that notify about their changes
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

#ifndef __CORE__NODE__NOTIFY_H__35072B50
#define __CORE__NODE__NOTIFY_H__35072B50

#include <core/std/memory.h>

#include <boost/signals2/signal.hpp>

namespace rainynite::core {

class AbstractNotify {
public:
    AbstractNotify() :
        changed_signal(),
        destroy_detector(std::make_shared<Null>())
    {}
    AbstractNotify(AbstractNotify const& /*other*/) :
        changed_signal(),
        destroy_detector(std::make_shared<Null>())
    {}
public:
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
        auto slot = decltype(changed_signal)::slot_type(f);
        slot.track_foreign(destroy_detector);
        return changed_signal.connect(slot);
    }

private:
    /**
     * Signal to subscribe to this object's changes
     */
    boost::signals2::signal<void()> changed_signal;

private:
    struct Null {};
    shared_ptr<Null> destroy_detector;
};

} // namespace rainynite::core

#endif
