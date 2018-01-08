/*  node_info/default_value.h - default nodes
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

#ifndef CORE_NODE_INFO_DEFAULT_NODE_H_9A3D8799_019A_526D_92F7_5571C1DEE0C0
#define CORE_NODE_INFO_DEFAULT_NODE_H_9A3D8799_019A_526D_92F7_5571C1DEE0C0

#include <core/node/common.h>
#include "node_info.h"

namespace rainynite::core {

AbstractReference make_default_node(Type type);

template <typename T>
AbstractReference make_default_node() {
    return make_default_node(typeid(T));
}

struct DefaultNodeInfo {
    virtual NodeInfo const& operator()() const = 0;
};

template <typename T, typename Node>
struct DefaultNodeInfoImpl :
    public DefaultNodeInfo,
    class_init::Registered<
        DefaultNodeInfoImpl<T, Node>,
        T,
        DefaultNodeInfo
    >
{
    NodeInfo const& operator()() const override {
        return get_node_info(typeid(Node));
    }
};

inline NodeInfo const& get_default_node_info(Type type) {
    return class_init::type_info<DefaultNodeInfo, NodeInfo const&>(type);
}

} // namespace rainynite::core

#endif
