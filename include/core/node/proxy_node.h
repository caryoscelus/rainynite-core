/*
 *  proxy_node.h - node redirecting to other nodes
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

#ifndef __CORE__NODE__PROXY_NODE_H__BFCE55CA
#define __CORE__NODE__PROXY_NODE_H__BFCE55CA

#include <functional>

#include "node.h"

namespace core {

/**
 * ProxyNode is a node that returns value of another node, possibly with changed context.
 *
 * Examples include various switch (e.g. Animated) and time-manipulating nodes.
 */
template <typename T>
class ProxyNode : public Node<T> {
public:
    /**
     * Call arbitrary function with correct child node and time.
     */
    virtual void step_into(std::shared_ptr<Context> context, std::function<void(NodeInContext)> f) const = 0;
public:
    T get(std::shared_ptr<Context> context) const override {
        try {
            T result;
            step_into(context, [&result](NodeInContext nic) {
                if (auto vnode = dynamic_cast<BaseValue<T>*>(nic.node.get()))
                    result = vnode->get(nic.context);
            });
            return result;
        } catch (...) {
            return {};
        }
    }
};

/**
 * Similar to ProxyNode, but for dynamic lists
 */
template <typename T>
class ProxyListNode : public Node<std::vector<T>> {
public:
    std::vector<T> get(std::shared_ptr<Context> ctx) const override {
        try {
            std::vector<T> result;
            this->step_into_list(ctx, [&result](NodeInContext nic) {
                if (auto vnode = dynamic_cast<BaseValue<T>*>(nic.node.get()))
                    result.push_back(vnode->get(nic.context));
            });
            return result;
        } catch (...) {
            return {};
        }
    }
};

} // namespace core

#endif
