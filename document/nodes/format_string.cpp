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
#include <core/node/node.h>
#include <core/node/property.h>

namespace rainynite::core::nodes {

static const size_t FORMAT_ARGS = 16;

template <size_t... i>
string format_vector(string format, vector<string> args, std::index_sequence<i...>) {
    return fmt::format(format, args[i]...);
}

class FormatString : public Node<string> {
public:
    FormatString() {
        init(format, string());
        init_list<string>(arguments);
    }
protected:
    string get(shared_ptr<Context> ctx) const override {
        try {
            auto str = get_format()->value(ctx);
            auto args = get_arguments()->value(ctx);
            if (args.size() > FORMAT_ARGS)
                throw std::runtime_error("FormatString: too much arguments");
            while (args.size() < FORMAT_ARGS) {
                args.emplace_back();
            }
            return format_vector(str, args, std::make_index_sequence<FORMAT_ARGS>());
        } catch (...) {
            // TODO: DEBUG
            return string();
        }
    }

private:
    NODE_PROPERTY(format, string);
    NODE_LIST_PROPERTY(arguments, string);
};

REGISTER_NODE(FormatString);

} // namespace rainynite::core::nodes
