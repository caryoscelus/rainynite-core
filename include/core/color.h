/*
 *  node.h - Node system
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

template <>
std::string to_hex24(RGBA<uint8_t> const& color) {
    return "#{:02x}{:02x}{:02x}"_format(color.r, color.g, color.b);
}

template <>
std::string to_hex32(RGBA<uint8_t> const& color) {
    return "#{:02x}{:02x}{:02x}{:02x}"_format(color.r, color.g, color.b, color.a);
}

RGBA32 parse_hex(std::string const& s) {
    RGBA32 color;
    color.a = 0xff;
    size_t sz = s.size()-1;
    if (s[0] != '#' || !(sz == 3 || sz == 4 || sz == 6 || sz == 8)) {
        throw std::runtime_error("Can't parse color '"+s+"'");
    }
    uint32_t c = std::stoul(s.c_str()+1, nullptr, 16);
    switch (sz) {
        case 4:
            color.a = (c & 0xf) * 1.0 * 0xff / 0xf;
            c >>= 4;
        case 3:
            color.b = (c & 0xf) * 1.0 * 0xff / 0xf;
            c >>= 4;
            color.g = (c & 0xf) * 1.0 * 0xff / 0xf;
            c >>= 4;
            color.r = (c & 0xf) * 1.0 * 0xff / 0xf;
            break;
        case 8:
            color.a = c & 0xff;
            c >>= 8;
        case 6:
            color.b = c & 0xff;
            c >>= 8;
            color.g = c & 0xff;
            c >>= 8;
            color.r = c & 0xff;
            break;
    }
    return color;
}

std::ostream& operator<<(std::ostream& stream, Color c) {
    return stream << to_hex32(c);
}

} // namespace color
} // namespace core

#endif
