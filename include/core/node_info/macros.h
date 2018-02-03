/*  node_info/macros.h - node registering macros
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

#ifndef CORE_NODE_INFO_MACROS_H_B6FB4E3D_5313_570A_9D5D_516D40639C2E
#define CORE_NODE_INFO_MACROS_H_B6FB4E3D_5313_570A_9D5D_516D40639C2E

#include "node_info.h"

#define NODE_INFO_PARENTS(Self, Type) \
public NodeInfo, \
private class_init::Registered<Self, Type, NodeInfo>, \
private class_init::ReverseRegistered<Self, Type, string>, \
private RegisterNodeByType<Self>

#define NODE_INFO_TEMPLATE(Name, Node, NodeType) \
template <typename T> \
struct Name##NodeInfo : NODE_INFO_PARENTS(Name##NodeInfo<T>, Node) { \
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
struct NodeNodeInfo : NODE_INFO_PARENTS(NodeNodeInfo, Node) { \
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

#define REGISTER_NODE_ALIAS(Node, AliasName, alias) \
struct AliasName : ReverseRegistered<AliasName, Node, AliasNameBase> { \
    string operator()() const override { \
        return alias; \
    } \
}

#endif
