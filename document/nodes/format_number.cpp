/*
 *  format_number.cpp - number format node
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

namespace rainynite::core::nodes {

template <typename N>
class FormatNumber : public Node<string> {
public:
    FormatNumber() {
        init<string>(format, {});
        init<double>(number, 0);
    }
public:
    string get(shared_ptr<Context> ctx) const override {
        auto str = get_format()->value(ctx);
        auto num = static_cast<N>(get_number()->value(ctx));
        return fmt::format(str, num);
    }

private:
    NODE_PROPERTY(format, string);
    NODE_PROPERTY(number, double);
};

using FormatInteger = FormatNumber<long long>;
using FormatReal = FormatNumber<double>;

REGISTER_NODE(FormatInteger);
REGISTER_NODE(FormatReal);

} // namespace rainynite::core::nodes
