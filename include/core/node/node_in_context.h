/*  node_in_context.h - node in context
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

#ifndef CORE_NODE_NODE_IN_CONTEXT_H_67AA500D_5DAB_5268_9265_5B16B7E57202
#define CORE_NODE_NODE_IN_CONTEXT_H_67AA500D_5DAB_5268_9265_5B16B7E57202

#include <core/std/memory.h>
#include <core/std/tuple.h>
#include <core/std/any.h>

namespace rainynite::core {

class AbstractValue;
class Context;

struct NodeInContext {
    shared_ptr<AbstractValue> node;
    shared_ptr<Context> context;

    NodeInContext() = default;
    NodeInContext(shared_ptr<AbstractValue> node_, shared_ptr<Context> context_) :
        node(node_),
        context(context_)
    {}

    operator bool() const {
        return node && context;
    }

    using NicPair = pair<shared_ptr<AbstractValue>, shared_ptr<Context>>;
    operator NicPair() const {
        return { node, context };
    }

    any value() const;

    template <typename T>
    T value_as() const {
        return any_cast<T>(value());
    }
};

} // namespace rainynite::core

#endif
