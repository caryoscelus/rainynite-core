/*
 *  node_in_context.h - node in context
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

#ifndef __CORE__NODE__NODE_IN_CONTEXT_H__4A18E5A4
#define __CORE__NODE__NODE_IN_CONTEXT_H__4A18E5A4

#include <memory>
#include <utility>

namespace rainynite::core {

class AbstractValue;
class Context;

struct NodeInContext {
    std::shared_ptr<AbstractValue> node;
    std::shared_ptr<Context> context;

    NodeInContext() = default;
    NodeInContext(std::shared_ptr<AbstractValue> node_, std::shared_ptr<Context> context_) :
        node(node_),
        context(context_)
    {}

    operator bool() const {
        return node && context;
    }

    using pair = std::pair<std::shared_ptr<AbstractValue>, std::shared_ptr<Context>>;
    operator pair() const {
        return { node, context };
    }
};

} // namespace rainynite::core

#endif
