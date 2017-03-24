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

#include <core/time/time_period.h>

namespace core {

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
