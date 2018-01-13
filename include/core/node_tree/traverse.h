/*  node_tree/traverse.h - node tree traverse
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

#ifndef CORE_NODE_TREE_TRAVERSE_H_E3ACBA14_B4A0_5ECF_9FE3_EC5D970C29E4
#define CORE_NODE_TREE_TRAVERSE_H_E3ACBA14_B4A0_5ECF_9FE3_EC5D970C29E4

#include <core/node/common.h>
#include <core/node/type_constraint.h>
#include <core/util/exceptions.h>
#include "node_tree.h"

namespace rainynite::core {

/**
 * Traverse node tree.
 */
class TreeTraverser {
public:
    TreeTraverser(NodeTree& tree_) :
        tree(tree_)
    {}

    void traverse_tree();
    void traverse_tree_from(NodeTree::Index index);

protected:
     /**
     * Called when traverser enters object.
     *
     * If this function returns true, object content is traversed, otherwise not.
     */
    virtual bool object_start(NodeTree::Index index) = 0;
    virtual void object_end(NodeTree::Index index) = 0;

    NodeTree& tree;
};

} // namespace rainynite::core

#endif
