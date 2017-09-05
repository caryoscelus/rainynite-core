/*
 *  parse.h - generic parsing stuff
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

#ifndef __CORE__PARSE_H__8DDA4A9A
#define __CORE__PARSE_H__8DDA4A9A

#include <stdexcept>

#include <core/std/string.h>

namespace rainynite::core {

class ParseError : public std::runtime_error {
public:
    ParseError(string const& msg) :
        std::runtime_error(msg)
    {}
};

} // namespace rainynite::core

#endif
