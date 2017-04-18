/*
 *  format.cpp - time format
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

#include <core/time/format.h>

namespace core {

std::ostream& operator<<(std::ostream& stream, Time time) {
    return stream << time.get_frames() << "f@" << time.get_fps();
}

std::ostream& operator<<(std::ostream& stream, TimePeriod const& period) {
    return stream << "[" << period.get_first() << "," << period.get_last() << ")";
}

} // namespace core