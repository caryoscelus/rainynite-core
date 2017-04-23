/*
 *  file_string.cpp - node reading file into string
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

#include <fstream>
#include <sstream>

#include <core/node.h>
#include <core/node_info.h>

namespace core {
namespace nodes {

REGISTER_NODE(FileString);

class FileString : public Node<std::string> {
public:
    FileString() {
        init<std::string>(file_name, "");
    }
public:
    virtual std::string get(Time time) const override {
        try {
            auto fname = get_file_name()->get(time);
            std::ifstream stream(fname);
            std::stringstream buffer;
            buffer << stream.rdbuf();
            return buffer.str();
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(file_name, std::string);
};

} // namespace nodes
} // namespace core
