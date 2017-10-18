/*  time_point.h - TimePoint which contains Time and node reference
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

#ifndef CORE_TIME_POINT_H_87D03CD2_5019_51AF_9947_B8787C8A4F04
#define CORE_TIME_POINT_H_87D03CD2_5019_51AF_9947_B8787C8A4F04

#include "time/time.h"
#include "node/abstract_value.h"

namespace rainynite::core {

template <typename T>
struct TimePoint {
    Time time;
    shared_ptr<BaseValue<T>> value;
};

} // namespace rainynite::core

#endif
