/*  format_string.cpp - string format node
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

#include <fmt/format.h>

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/node/list.h>

namespace rainynite::core::nodes {

static const size_t FORMAT_ARGS = 16;

template <size_t... i>
string format_vector(string format, vector<string> args, std::index_sequence<i...>) {
    return fmt::format(format, args[i]...);
}

class FormatString :
    public NewNode<
        FormatString,
        string,
        types::Only<string>,
        types::Only<vector<string>>
    >
{
    DOC_STRING(
        "Format string (using fmt library)"
        "\n\n"
        "(amount of arguments is limited)"
    )

    NODE_PROPERTIES("format", "arguments")
    COMPLEX_DEFAULT_VALUES(make_value<string>(), make_node<ListValue<string>>())
    PROPERTY(format)
    PROPERTY(arguments)

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto str = format_value<string>(ctx);
        auto args = arguments_value<vector<string>>(ctx);
        if (args.size() > FORMAT_ARGS)
            throw std::runtime_error("FormatString: too much arguments");
        while (args.size() < FORMAT_ARGS) {
            args.emplace_back();
        }
        return format_vector(str, args, std::make_index_sequence<FORMAT_ARGS>());
    }
};

REGISTER_NODE(FormatString);

} // namespace rainynite::core::nodes
