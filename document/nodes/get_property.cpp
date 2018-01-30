/*  get_property.cpp - extract property from child node
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

#include <core/node_info/macros.h>
#include <core/node/proxy_node.h>
#include <core/node/cast.h>
#include <core/all_types.h>

namespace rainynite::core::nodes {

template <typename T>
class GetProperty :
    public NewProxyNode<
        GetProperty<T>,
        T,
        types::Any,
        types::Only<string>
    >
{
    DOC_STRING(
        "Extract property from its child"
    )

    NODE_PROPERTIES("source", "property_name")
    DEFAULT_VALUES(Nothing{}, string{})
    PROPERTY(source)
    PROPERTY(property_name)

protected:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        if (auto node = abstract_node_cast(p_source())) {
            if (auto property_name = property_name_value<string>(ctx); !property_name.empty()) {
                return {node->get_property(property_name), ctx};
            }
        }
        throw NodeAccessError("GetProperty failure");
    }
};

NODE_INFO_INSTANCES(GetProperty, GetProperty<T>, T)

} // namespace rainynite::core::nodes
