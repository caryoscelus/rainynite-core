/*
 *  node_info.h - runtime node information
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

#ifndef __CORE__NODE_INFO_H__1D5DEEF0
#define __CORE__NODE_INFO_H__1D5DEEF0

#include <core/class_init.h>
#include <core/node.h>

namespace core {

/**
 * Runtime node information interface.
 */
class NodeInfo {
public:
    virtual std::string operator()() const = 0;
    virtual AbstractReference new_empty() const = 0;
};

inline std::string node_type_name(std::type_index type) {
    try {
        return class_init::type_info<NodeInfo,std::string>(type);
    } catch (class_init::RuntimeTypeError const& ex) {
        return "unknown";
    }
}

inline std::string node_name(AbstractReference node) {
    return node_type_name(typeid(*node));
}

inline NodeInfo const& get_node_type(std::string const& name) {
    auto type = class_init::find_type(name);
    return class_init::type_meta<NodeInfo>(type);
}

template <typename T>
std::shared_ptr<T> make_node_with_name(std::string const& name, boost::any const& value = boost::any()) {
    auto node = get_node_type(name).new_empty();
    if (!value.empty() && node->can_set_any(value))
        node->set_any(value);
    return std::dynamic_pointer_cast<T>(node);
}

#define REGISTER_NODE_NAMED(Node, name) \
class Node; \
class Node##NodeInfo : public NodeInfo, class_init::Registered<Node##NodeInfo, Node, NodeInfo>, class_init::ReverseRegistered<Node##NodeInfo, Node, std::string> { \
public: \
    virtual std::string operator()() const override { \
        return name; \
    } \
    virtual AbstractReference new_empty() const override { \
        return std::static_pointer_cast<AbstractValue>(std::make_shared<Node>()); \
    } \
}

#define REGISTER_NODE(Node) REGISTER_NODE_NAMED(Node, #Node)

} // namespace core

#endif
