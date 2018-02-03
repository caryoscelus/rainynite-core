/*  renderable_node.h - renderable node & Empty
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

#ifndef CORE_RENDERABLE_NODE_H_1EC6DC54_CD77_51B1_B90F_EAE99727BE6D
#define CORE_RENDERABLE_NODE_H_1EC6DC54_CD77_51B1_B90F_EAE99727BE6D

#include <core/node/new_node.h>
#include "renderable.h"

namespace rainynite::core {

/**
 * Base class for all renderable nodes.
 *
 * Since Renderable is only tag class, the point of this base class
 * is to provide empty get() override.
 */
template <class Self, typename... Ts>
class RenderableNode :
    public NewNode<
        Self,
        Renderable,
        Ts...
    >
{
protected:
    Renderable get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
};

class Empty : public RenderableNode<Empty> {
    DOC_STRING(
        "Empty renderable"
    )

    NODE_PROPERTIES()
    DEFAULT_VALUES()
};

} // namespace rainynite::core

#endif
