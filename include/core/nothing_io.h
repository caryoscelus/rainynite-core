/*  nothing_io.h - Nothing io
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

#ifndef CORE_NOTHING_IO_H_11CC02AE_A8BF_544F_9DB8_2A591229833C
#define CORE_NOTHING_IO_H_11CC02AE_A8BF_544F_9DB8_2A591229833C

#include <iostream>

#include "nothing.h"

namespace rainynite {

inline std::ostream& operator<<(std::ostream& s, Nothing) {
    return s << "Nothing";
}

} // namespace rainynite

#endif
