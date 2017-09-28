/*  exceptions.h - serialization exceptions
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

#ifndef CORE_SERIALIZE_EXCEPTIONS_H_9B2D6ABE_DA34_54D7_80A4_CB39480568B3
#define CORE_SERIALIZE_EXCEPTIONS_H_9B2D6ABE_DA34_54D7_80A4_CB39480568B3

#include <stdexcept>

#include <core/std/string.h>

namespace rainynite::core::serialize {

class SerializationError : public std::runtime_error {
public:
    SerializationError(string const& msg) :
        std::runtime_error(msg)
    {}
};

class DeserializationError : public SerializationError {
public:
    DeserializationError(string const& msg) :
        SerializationError(msg)
    {}
};

} // namespace rainynite::core::serialize

#endif
