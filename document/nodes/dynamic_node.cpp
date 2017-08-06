/*
 *  dynamic_node.cpp - create node from argument list
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/node/proxy_node.h>
#include <core/all_types.h>

namespace core {
namespace nodes {

template <typename T>
class DynamicNode : public ProxyNode<T> {
public:
    DynamicNode() {
        this->template init<std::string>(node_type, {});
        this->template init_property(arguments, boost::make_optional(Type(typeid(Nothing))), std::make_shared<UntypedListValue>());
    }
public:
    void step_into(Time time, std::function<void(AbstractReference,Time)> f) const override {
        try {
            auto node = make_node_with_name<AbstractListLinked>(get_node_type()->get(time));
            size_t i = 0;
            for (auto const& arg : list_arguments()->get_links()) {
                node->set_link(i, arg);
                ++i;
            }
            f(std::dynamic_pointer_cast<AbstractValue>(node), time);
        } catch (...) {
        }
    }

private:
    NODE_PROPERTY(node_type, std::string);
    NODE_LIST_PROPERTY(arguments, Nothing);
};

NODE_INFO_TEMPLATE(DynamicNode, DynamicNode<T>, std::vector<T>);
TYPE_INSTANCES(DynamicNodeNodeInfo)

} // namespace nodes
} // namespace core
