/*  join.cpp - merge list into string
 *  Copyright (C) 2018 caryoscelus
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

#include <boost/algorithm/string/join.hpp>

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/node/list.h>

namespace rainynite::core::nodes {

class JoinStringList :
    public NewNode<
        JoinStringList,
        string,
        types::Only<string>,
        types::Only<vector<string>>
    >
{
    DOC_STRING(
        "Merge list into string with separator."
    )

    NODE_PROPERTIES("separator", "strings")
    COMPLEX_DEFAULT_VALUES(make_value<string>(), make_node<ListValue<string>>())

    PROPERTY(separator)
    PROPERTY(strings)

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto strings = strings_value<vector<string>>(ctx);
        auto separator = separator_value<string>(ctx);
        return boost::algorithm::join(strings, separator);
    }
};

REGISTER_NODE(JoinStringList);

} // namespace rainynite::core::nodes
