/*  proxy_node.h - node redirecting to other nodes
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

#ifndef CORE_NODE_PROXY_NODE_H_E46910AC_D319_5AB4_AB28_F69FAF7A6767
#define CORE_NODE_PROXY_NODE_H_E46910AC_D319_5AB4_AB28_F69FAF7A6767

#include <functional>

#include "node.h"

namespace rainynite::core {

/**
 * ProxyNode is a node that returns value of another node, possibly with changed context.
 *
 * Examples include various switch (e.g. Animated) and time-manipulating nodes.
 */
template <typename T>
class ProxyNode : public Node<T> {
public:
    /**
     * Get proxied NodeInContext
     */
    virtual NodeInContext get_proxy(shared_ptr<Context> ctx) const = 0;
public:
    T get(shared_ptr<Context> ctx) const override {
        auto [node, nctx] = get_proxy(ctx);
        if (auto vnode = dynamic_cast<BaseValue<T>*>(node.get()))
            return vnode->get(nctx);
        throw NodeAccessError("Proxied node is of different type");
    }
};

} // namespace rainynite::core

#endif
