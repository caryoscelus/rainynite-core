/*
 *  text.cpp - Text render node
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
#include <core/renderable.h>
#include <core/color.h>

namespace core {
namespace nodes {

class Text : public Node<Renderable> {
public:
    Text() {
        init<std::string>(text, "");
        init<colors::Color>(color, {});
        init<double>(size, 0);
    }
public:
    virtual Renderable get(Time time) const override {
        return Renderable();
    }
private:
    NODE_PROPERTY(text, std::string);
    NODE_PROPERTY(color, colors::Color);
    NODE_PROPERTY(size, double);
};

REGISTER_NODE(Text);

} // namespace nodes
} // namespace core
