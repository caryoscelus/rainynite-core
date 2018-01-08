/*  string.cpp - string manipulation nodes
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

#include <core/node_info.h>
#include <core/node/new_node.h>

namespace rainynite::core::nodes {

class SubString :
    public NewNode<
        SubString,
        string,
        types::Only<string>,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Take a substring.\n"
    )

    NODE_PROPERTIES("source", "first", "count")
    DEFAULT_VALUES(string{}, 0.0, 0.0);

    PROPERTY(source)
    PROPERTY(first)
    PROPERTY(count)

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto s = source_value<string>(ctx);
        auto first = std::floor(first_value<double>(ctx));
        ptrdiff_t idx = first < 0 ? s.size()+first : first;
        if (idx < 0)
            return {};
        size_t count = std::fmax(0, count_value<double>(ctx));
        return s.substr(idx, count);
    }

};

REGISTER_NODE(SubString);

} // namespace rainynite::core::nodes
