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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

namespace core {
namespace nodes {

class FileString : public Node<std::string> {
public:
    FileString() {
        init<std::string>(file_name, "");
    }
public:
    std::string get(std::shared_ptr<Context> ctx) const override {
        try {
            auto fname = get_file_name()->get(ctx);
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

REGISTER_NODE(FileString);

} // namespace nodes
} // namespace core
