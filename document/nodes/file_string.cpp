/*  file_string.cpp - node reading file into string
 *  Copyright (C) 2017-2018 caryoscelus
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

namespace rainynite::core::nodes {

class FileString :
    public NewNode<
        FileString,
        string,
        types::Only<string>
    >
{
    DOC_STRING(
        "Load file into string"
    )

    NODE_PROPERTIES("file_name")
    DEFAULT_VALUES(string{})
    PROPERTY(file_name)

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto fname = file_name_value<string>(ctx);
        std::ifstream stream(fname);
        std::stringstream buffer;
        buffer << stream.rdbuf();
        return buffer.str();
    }
};

REGISTER_NODE(FileString);

} // namespace rainynite::core::nodes
