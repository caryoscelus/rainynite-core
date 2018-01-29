/*  replace_context.h - replace context node helper
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

#ifndef CORE_NODE_REPLACE_CONTEXT_H_27310958_4CCA_5E62_B6DE_1A4E90656967
#define CORE_NODE_REPLACE_CONTEXT_H_27310958_4CCA_5E62_B6DE_1A4E90656967

#include <core/all_types.h>
#include "proxy_node.h"

namespace rainynite::core {

/**
 * Node that replaces context for its child node.
 *
 * Currently can only be used internally.
 */
template <typename T>
class ReplaceContextNode : public ProxyNode<T> {
public:
    ReplaceContextNode(NodeInContext nic_) :
        nic(nic_)
    {}

    NodeInContext get_proxy(shared_ptr<Context> /*ctx*/) const override {
        return nic;
    }

    AbstractReference get_property(string const& name) const override {
        if (auto node = abstract_node_cast(nic.node))
            return node->get_property(name);
        throw NodeAccessError("no proxied property");
    }

private:
    NodeInContext const nic;
};

shared_ptr<AbstractValue> make_replace_context_node(Type type, NodeInContext nic);

} // namespace rainynite::core

#endif
