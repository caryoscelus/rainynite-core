/*
 *  to_string.cpp - node converting any value to string
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
#include <core/serialize/node_writer.h>

namespace core {
namespace nodes {

REGISTER_NODE(ToString);

class ToString : public Node<std::string> {
public:
    ToString() {
        init<std::string>(source, "");
    }
public:
    virtual std::string get(Time time) const override {
        try {
            auto v = source.get()->get_any(time);
            return serialize::value_to_string(v);
        } catch (...) {
            return {};
        }
    }

private:
    NodeProperty source {"source", ""};
};

} // namespace nodes
} // namespace core
