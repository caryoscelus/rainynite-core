/*  split_string.cpp - node to split string into string list
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

#include <boost/algorithm/string.hpp>

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

namespace rainynite::core::nodes {

class SplitString :
    public NewNode<
        SplitString,
        vector<string>,
        types::Only<string>,
        types::Only<string>
    >
{
    DOC_STRING(
        "Split string into list"
    )

    NODE_PROPERTIES("source", "split")
    DEFAULT_VALUES(string{}, string{"\n"})
    PROPERTY(source)
    PROPERTY(split)

protected:
    vector<string> get(shared_ptr<Context> ctx) const override {
        vector<string> result;
        auto s = source_value<string>(ctx);
        auto split = split_value<string>(ctx);
        boost::split(result, s, boost::is_any_of(split));
        return result;
    }

    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        vector<NodeInContext> result;
        auto list = value(ctx);
        std::transform(
            std::begin(list),
            std::end(list),
            std::back_inserter(result),
            [ctx](auto&& s) -> NodeInContext {
                return { make_value<string>(s), ctx };
            }
        );
        return result;
    }
};

REGISTER_NODE(SplitString);

} // namespace rainynite::core::nodes
