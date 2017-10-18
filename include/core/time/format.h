/*  time/format.h - time format
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

#ifndef CORE_TIME_FORMAT_H_73239C60_026C_5D2A_AD87_F7880896AB66
#define CORE_TIME_FORMAT_H_73239C60_026C_5D2A_AD87_F7880896AB66

#include <ostream>

#include "period.h"

namespace rainynite::core {

std::ostream& operator<<(std::ostream& stream, Time time);
std::ostream& operator<<(std::ostream& stream, TimePeriod const& period);

} // rainynite::core

#endif
