/*  destroy_detector.h - report object destruction for boost signal system
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

#ifndef CORE_DESTROY_DETECTOR_H_DD71BC27_C341_5CF3_8E0B_71C19AE585A2
#define CORE_DESTROY_DETECTOR_H_DD71BC27_C341_5CF3_8E0B_71C19AE585A2

#include <core/std/memory.h>
#include <core/nothing.h>

namespace rainynite {

/**
 * A base class that allows tracking connection to boost signals.
 *
 * Note that using enable_shared_from_this seems like a attractive alternative
 * to empty destroy_detector, but it is more limited: it won't work when
 * instance is placed on stack or owned by unique_ptr.
 */
class DestroyDetector {
public:
    DestroyDetector() :
        destroy_detector(make_shared<Nothing>())
    {}

    DestroyDetector(DestroyDetector const& /*other*/) :
        destroy_detector(make_shared<Nothing>())
    {}

    virtual ~DestroyDetector() {
    }

    /**
     * Connect signal and make connection track this object.
     */
    template <class S, class F>
    auto connect_boost(S& signal, F lambda) {
        auto slot = typename S::slot_type(lambda);
        slot.track_foreign(destroy_detector);
        return signal.connect(slot);
    }

private:
    shared_ptr<Nothing> const destroy_detector;
};

} // namespace rainynite

#endif
