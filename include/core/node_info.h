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
    std::string operator()() const {
        return name();
    }
    virtual std::string name() const = 0;
    virtual AbstractReference new_empty() const = 0;
    virtual AbstractReference clone(AbstractValue const& source) const = 0;
    virtual Type type() const = 0;
};

inline NodeInfo const& get_node_info(std::type_index type) {
    return class_init::type_meta<NodeInfo>(type);
}

inline std::string node_type_name(std::type_index type) {
    try {
        return class_init::type_info<NodeInfo,std::string>(type);
    } catch (class_init::RuntimeTypeError const& ex) {
        return "unknown";
    }
}

inline std::string node_name(AbstractValue const& node) {
    return node_type_name(typeid(node));
}

inline NodeInfo const& get_node_type(std::string const& name) {
    return get_node_info(class_init::find_type(name));
}

template <typename T>
std::shared_ptr<T> make_node_with_name(std::string const& name, boost::any const& value = boost::any()) {
    auto node = get_node_type(name).new_empty();
    node->new_id(); // TODO: don't do that here?
    if (!value.empty() && node->can_set_any(value))
        node->set_any(value);
    return std::dynamic_pointer_cast<T>(node);
}

inline AbstractReference shallow_copy(AbstractValue const& source) {
    auto node = get_node_info(typeid(source)).clone(source);
    node->new_id();
    return node;
}

inline std::map<Type, std::set<NodeInfo const*>>& node_types() {
    static std::map<Type, std::set<NodeInfo const*>> instance;
    return instance;
}

template <class I>
struct RegisterNodeByType : class_init::Initialized<RegisterNodeByType<I>> {
    static void init() {
        auto info = new I();
        node_types()[info->type()].insert(info);
    }
};

#define NODE_INFO_PARENTS(Self, Type) \
public NodeInfo, \
private class_init::Registered<Self, Type, NodeInfo>, \
private class_init::ReverseRegistered<Self, Type, std::string>, \
private RegisterNodeByType<Self>

#define REGISTER_NODE_NAMED(Node, NodeNodeInfo, _name) \
class NodeNodeInfo : NODE_INFO_PARENTS(NodeNodeInfo, Node) \
{ \
public: \
    virtual std::string name() const override { \
        return _name; \
    } \
    virtual AbstractReference new_empty() const override { \
        return std::static_pointer_cast<AbstractValue>(std::make_shared<Node>()); \
    } \
    virtual AbstractReference clone(AbstractValue const& source) const override { \
        return std::make_shared<Node>(static_cast<Node const&>(source)); \
    } \
    virtual core::Type type() const override { \
        return Node::static_type(); \
    } \
}

#define REGISTER_NODE(Node) \
class Node; \
REGISTER_NODE_NAMED(Node, Node##NodeInfo, #Node)

} // namespace core

#endif
