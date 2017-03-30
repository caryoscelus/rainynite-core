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

class NodeInfo {
public:
    virtual std::string operator()() const = 0;
};

std::string node_name(AbstractReference node) {
    try {
        return class_init::type_info<NodeInfo,std::string>(typeid(*node));
    } catch (class_init::RuntimeTypeError const& ex) {
        return "unknown";
    }
}

#define REGISTER_NODE_NAMED(Node, name) \
class Node##NodeInfo : public NodeInfo, class_init::Registered<Node##NodeInfo, Node, NodeInfo> { \
public: \
    virtual std::string operator()() const override { \
        return name; \
    } \
}

#define REGISTER_NODE(Node) REGISTER_NODE_NAMED(Node, #Node)

} // namespace core

#endif
