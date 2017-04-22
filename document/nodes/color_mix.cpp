/*
 *  color_mix.cpp - color mixing node
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

#include <core/node.h>
#include <core/node_info.h>
#include <core/color.h>

namespace core {
namespace nodes {

REGISTER_NODE(ColorMix);

class ColorMix : public Node<colors::Color> {
public:
    ColorMix() {
        init(a, colors::Color());
        init(b, colors::Color());
    }
public:
    virtual colors::Color get(Time time) const override {
        auto a = get_a()->get(time);
        auto b = get_b()->get(time);
        auto t = time.get_seconds();
        return colors::mix(a, b, t);
    }

private:
    NODE_PROPERTY(a, colors::Color);
    NODE_PROPERTY(b, colors::Color);
};

} // namespace nodes
} // namespace core
