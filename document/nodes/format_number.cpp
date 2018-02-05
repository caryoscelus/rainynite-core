/*  format_number.cpp - number format node
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

namespace rainynite::core::nodes {

template <typename N>
class FormatNumber :
    public NewNode<
        FormatNumber<N>,
        string,
        types::Only<string>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Format number to string."
    )

    NODE_PROPERTIES("format", "number")
    DEFAULT_VALUES(string{}, 0.0)
    PROPERTY(format)
    PROPERTY(number)

protected:
    string get(shared_ptr<Context> ctx) const override {
        auto str = format_value<string>(ctx);
        auto num = static_cast<N>(number_value<double>(ctx));
        return fmt::format(str, num);
    }
};

using FormatReal = FormatNumber<double>;

REGISTER_NODE(FormatReal);

} // namespace rainynite::core::nodes
