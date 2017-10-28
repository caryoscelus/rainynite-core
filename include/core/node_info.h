/*  node_info.h - runtime node information
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

#ifndef CORE_NODE_INFO_H_E59BD832_E858_5D6E_83EC_79AF65B01A25
#define CORE_NODE_INFO_H_E59BD832_E858_5D6E_83EC_79AF65B01A25

#include <core/std/set.h>
#include <core/class_init.h>
#include <core/type_info.h>
#include <core/node/abstract_value.h>

namespace rainynite::core {

/**
 * Runtime node information interface.
 */
class NodeInfo {
public:
    string operator()() const {
        return name();
    }
    virtual string name() const = 0;
    virtual AbstractReference new_empty() const = 0;
    virtual AbstractReference clone(AbstractValue const& source) const = 0;
    virtual Type type() const = 0;
};

inline NodeInfo const& get_node_info(std::type_index type) {
    return class_init::type_meta<NodeInfo>(type);
}

/// Get node name for given type if it's a type of registered node
inline string node_type_name(std::type_index type) {
    try {
        return class_init::type_info<NodeInfo,string>(type);
    } catch (class_init::RuntimeTypeError const& ex) {
        return "unknown";
    }
}

/// Get node name
inline string node_name(AbstractValue const& node) {
    return node_type_name(typeid(node));
}

inline NodeInfo const& get_node_type(string const& name) {
    return get_node_info(class_init::find_type(name));
}

/**
 * Make node with given name, optionally using source & context to initialize it.
 *
 * TODO: throw instead of returning nullptr?
 */
template <typename T=AbstractValue>
shared_ptr<T> make_node_with_name(string const& name, AbstractReference source=nullptr, shared_ptr<Context> context=nullptr) {
    auto node = get_node_type(name).new_empty();
    node->new_id(); // TODO: don't do that here?
    if (source && context) {
        auto value = source->get_any(context);
        if (node->can_set_source(source))
            node->set_source(source);
        else if (source && node->can_set_any(value))
            node->set_any(value);
    }
    return dynamic_pointer_cast<T>(node);
}

inline AbstractReference shallow_copy(AbstractValue const& source) {
    auto node = get_node_info(typeid(source)).clone(source);
    node->new_id();
    return node;
}

template <class T>
shared_ptr<T> shallow_copy_as(AbstractValue const& source) {
    return dynamic_pointer_cast<T>(shallow_copy(source));
}

inline map<Type, set<NodeInfo const*>>& node_types() {
    static map<Type, set<NodeInfo const*>> instance;
    return instance;
}

inline set<NodeInfo const*> all_node_infos() {
    set<NodeInfo const*> result;
    for (auto const& e : node_types()) {
        result.insert(e.second.begin(), e.second.end());
    }
    return result;
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
private class_init::ReverseRegistered<Self, Type, string>, \
private RegisterNodeByType<Self>

#define NODE_INFO_TEMPLATE(Name, Node, NodeType) \
template <typename T> \
class Name##NodeInfo : NODE_INFO_PARENTS(Name##NodeInfo<T>, Node) { \
public: \
    string name() const override { \
        return #Name"/"+get_primitive_type_name<T>(); \
    } \
    AbstractReference new_empty() const override { \
        return make_shared<Node>(); \
    } \
    AbstractReference clone(AbstractValue const& source) const override { \
        return make_shared<Node>(static_cast<Node const&>(source)); \
    } \
    Type type() const override { \
        return typeid(NodeType); \
    } \
}

#define REGISTER_NODE_NAMED(Node, NodeNodeInfo, _name) \
class NodeNodeInfo : NODE_INFO_PARENTS(NodeNodeInfo, Node) \
{ \
public: \
    string name() const override { \
        return _name; \
    } \
    AbstractReference new_empty() const override { \
        return static_pointer_cast<AbstractValue>(make_shared<Node>()); \
    } \
    AbstractReference clone(AbstractValue const& source) const override { \
        return make_shared<Node>(static_cast<Node const&>(source)); \
    } \
    rainynite::core::Type type() const override { \
        return Node::static_type(); \
    } \
}

#define REGISTER_NODE(Node) \
REGISTER_NODE_NAMED(Node, Node##NodeInfo, #Node)

} // namespace rainynite::core

#endif
