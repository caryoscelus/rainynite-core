/*  color.h - Color types
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

#ifndef CORE_COLOR_H_57293FC5_57D6_5A47_9442_B89C021D2DF4
#define CORE_COLOR_H_57293FC5_57D6_5A47_9442_B89C021D2DF4

#include <cstdint>
#include <limits>

#include <fmt/format.h>

#include <core/std/string.h>
#include <core/std/algorithm.h>

using namespace fmt::literals;

namespace rainynite::core::colors {

enum class ConversionStyle {
    NonStrict,
    Strict,
    Default = Strict
};

/**
 * Abstract color type
 */
class AbstractColor {
public:
    virtual ~AbstractColor() = default;

    virtual double red() const = 0;
    virtual double green() const = 0;
    virtual double blue() const = 0;
    virtual double alpha() const = 0;
};

template <typename T>
class RGBA : public AbstractColor {
public:
    using Lim = std::numeric_limits<T>;

public:
    RGBA(T r_, T g_, T b_, T a_ = Lim::max()) :
        r(r_), g(g_), b(b_), a(a_)
    {}
    RGBA() :
        RGBA(0, 0, 0, Lim::max())
    {}
    virtual ~RGBA() {}
public:
    static RGBA<T> from_rgba(double r, double g, double b, double a=1.0, ConversionStyle conv=ConversionStyle::Default) {
        bool strict = conv == ConversionStyle::Strict;
        auto c = [strict](auto x) {
            if (strict)
                return clamp(x, 0.0, 1.0);
            else
                return x;
        };
        return RGBA<T>(
            c(r)*Lim::max(),
            c(g)*Lim::max(),
            c(b)*Lim::max(),
            c(a)*Lim::max()
        );
    }
public:
    double red() const override {
        return r * 1.0 / Lim::max();
    }
    double green() const override {
        return g * 1.0 / Lim::max();
    }
    double blue() const override {
        return b * 1.0 / Lim::max();
    }
    double alpha() const override {
        return a * 1.0 / Lim::max();
    }
public:
    bool operator==(RGBA<T> const& other) const {
        // different colors considered different even when alpha is zero
        return a == other.a
            && r == other.r
            && g == other.g
            && b == other.b;
    }
    bool operator!=(RGBA<T> const& other) const {
        return !(*this == other);
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
string to_hex24(C const& color);

template <class C>
string to_hex32(C const& color);

RGBA32 parse_hex(string const& s);

std::ostream& operator<<(std::ostream& stream, Color c);

} // namespace rainynite::core::colors

#endif
