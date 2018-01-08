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
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/all_types.h>

namespace rainynite::core::nodes {

template <typename T>
class IfElse : public ProxyNode<T> {
    DOC_STRING(
        "Node that takes value of one of its children based on condition."
    )
public:
    IfElse() {
        this->template init<bool>(condition, {});
        this->template init<T>(on_true, {});
        this->template init<T>(on_false, {});
    }
public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        if (get_condition()->value(ctx)) {
            return { get_on_true(), ctx };
        } else {
            return { get_on_false(), ctx };
        }
    }

private:
    NODE_PROPERTY(condition, bool);
    NODE_PROPERTY(on_true, T);
    NODE_PROPERTY(on_false, T);
};

NODE_INFO_TEMPLATE(IfElse, IfElse<T>, T);
TYPE_INSTANCES(IfElseNodeInfo)

} // namespace rainynite::core::nodes
