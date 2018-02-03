/*  conditional.cpp - conditional nodes
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

#include <core/node_info/macros.h>
#include <core/node_info/default_node.h>
#include <core/node/proxy_node.h>
#include <core/all_types.h>

namespace rainynite::core::nodes {

template <typename T>
class IfElse :
    public NewProxyNode<
        IfElse<T>,
        T,
        types::Only<bool>,
        types::Only<T>,
        types::Only<T>
    >
{
    DOC_STRING(
        "Node that takes value of one of its children based on condition."
    )

    NODE_PROPERTIES("condition", "on_true", "on_false")
    COMPLEX_DEFAULT_VALUES(make_value<bool>(), make_default_node<T>(), make_default_node<T>())
    PROPERTY(condition)
    PROPERTY(on_true)
    PROPERTY(on_false)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        if (condition_value<bool>(ctx)) {
            return { p_on_true(), ctx };
        } else {
            return { p_on_false(), ctx };
        }
    }
};

NODE_INFO_TEMPLATE(IfElse, IfElse<T>, T);
TYPE_INSTANCES(IfElseNodeInfo)

} // namespace rainynite::core::nodes
