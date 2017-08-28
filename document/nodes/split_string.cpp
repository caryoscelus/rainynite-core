/*
 *  split_string.cpp - node to split string into string list
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

#include <boost/algorithm/string.hpp>

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

namespace core {
namespace nodes {

class SplitString : public Node<std::vector<std::string>> {
public:
    SplitString() {
        init<std::string>(source, "");
        init<std::string>(split, "\n");
    }
public:
    std::vector<NodeInContext> get_list_links(std::shared_ptr<Context> ctx) const override {
        std::vector<NodeInContext> result;
        try {
            auto list = get(ctx);
            std::transform(
                std::begin(list),
                std::end(list),
                std::back_inserter(result),
                [ctx](auto&& s) -> NodeInContext {
                    return { make_value<std::string>(s), ctx };
                }
            );
        } catch (...) {
        }
        return result;
    }
    std::vector<std::string> get(std::shared_ptr<Context> ctx) const override {
        try {
            std::vector<std::string> result;
            auto s = get_source()->get(ctx);
            auto split = get_split()->get(ctx);
            boost::split(result, s, boost::is_any_of(split));
            return result;
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(source, std::string);
    NODE_PROPERTY(split, std::string);
};

REGISTER_NODE(SplitString);

} // namespace nodes
} // namespace core