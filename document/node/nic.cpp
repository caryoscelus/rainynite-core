/*  nic.cpp - node in context
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

#include <core/node/replace_context.h>

namespace rainynite::core {

any NodeInContext::value() const {
    return node->get_any(context);
}

struct ReplaceContextNodeFactory {
    virtual shared_ptr<AbstractValue> operator()(NodeInContext nic) const = 0;
};

template <typename T>
struct ReplaceContextNodeFactoryImpl :
    public ReplaceContextNodeFactory,
    class_init::Registered<ReplaceContextNodeFactoryImpl<T>, T, ReplaceContextNodeFactory>
{
    shared_ptr<AbstractValue> operator()(NodeInContext nic) const override {
        return make_shared<ReplaceContextNode<T>>(nic);
    }
};

TYPE_INSTANCES(ReplaceContextNodeFactoryImpl)

shared_ptr<AbstractValue> make_replace_context_node(Type type, NodeInContext nic) {
    return class_init::type_info<ReplaceContextNodeFactory, shared_ptr<AbstractValue>>(type, nic);
}

} // namespace rainynite::core
