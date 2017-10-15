/*  exceptions.h - common exceptions
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

#ifndef CORE_EXCEPTIONS_H_FA533FB1_76E7_51B9_A5B4_EE4A25D88CC9
#define CORE_EXCEPTIONS_H_FA533FB1_76E7_51B9_A5B4_EE4A25D88CC9

#include <stdexcept>

#include <core/std/string.h>

namespace rainynite {

struct NullPointerException : public std::runtime_error {
    NullPointerException(string const& msg) :
        runtime_error(msg)
    {}
};

} // namespace rainynite

#endif
