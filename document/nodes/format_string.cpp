/*
 *  format_string.cpp - string format node
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

#include <fmt/format.h>

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

#define FORMAT_ARGS 16

namespace core {
namespace nodes {

template <size_t... i>
std::string format_vector(std::string format, std::vector<std::string> args, std::index_sequence<i...>) {
    return fmt::format(format, args[i]...);
}

class FormatString : public Node<std::string> {
public:
    FormatString() {
        init(format, std::string());
        init_list<std::string>(arguments);
    }
public:
    virtual std::string get(Time time) const override {
        try {
            auto str = get_format()->get(time);
            auto args = get_arguments()->get(time);
            if (args.size() > FORMAT_ARGS)
                throw std::runtime_error("FormatString: too much arguments");
            while (args.size() < FORMAT_ARGS) {
                args.emplace_back();
            }
            return format_vector(str, args, std::make_index_sequence<FORMAT_ARGS>());
        } catch (...) {
            // TODO: DEBUG
            return std::string();
        }
    }

private:
    NODE_PROPERTY(format, std::string);
    NODE_LIST_PROPERTY(arguments, std::string);
};

REGISTER_NODE(FormatString);

} // namespace nodes
} // namespace core
