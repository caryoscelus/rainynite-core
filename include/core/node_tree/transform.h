/*  node_tree/transform.h - calculate transform of node tree elements
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

#ifndef CORE_NODE_TREE_TRANSFORM_H_5BE454D6_C46E_541A_A1A5_3EA70F024D0B
#define CORE_NODE_TREE_TRANSFORM_H_5BE454D6_C46E_541A_A1A5_3EA70F024D0B

#include <2geom/affine.h>

#include <core/node/common.h>
#include "node_tree.h"

namespace rainynite::core {

class Context;

Geom::Affine get_transform(shared_ptr<Context> ctx, NodeTree const& tree, NodeTree::Index index);

} // namespace rainynite::core

#endif
