/*
 *  color.h - Color types
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

#ifndef __CORE__COLOR_H__36A9AB66
#define __CORE__COLOR_H__36A9AB66

#include <cstdint>
#include <limits>
#include <string>

#include <fmt/format.h>

using namespace fmt::literals;

namespace core {
namespace colors {

class AbstractColor {
};

template <typename T>
class RGBA : public AbstractColor {
public:
    RGBA(T r_, T g_, T b_, T a_ = 0) :
        r(r_), g(g_), b(b_), a(a_)
    {}
    RGBA() :
        RGBA(0, 0, 0, 0)
    {}
public:
    double get_alpha() const {
        return a * 1.0 / std::numeric_limits<T>::max();
    }
public:
    T r;
    T g;
    T b;
    T a;
};

using RGBA32 = RGBA<uint8_t>;
using Color = RGBA32;

template <class C>
std::string to_hex24(C const& color);

template <class C>
std::string to_hex32(C const& color);

RGBA32 parse_hex(std::string const& s);

std::ostream& operator<<(std::ostream& stream, Color c);

} // namespace color
} // namespace core

#endif
