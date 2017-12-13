/*  node_tree_traverse.h - node tree traverse
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

#ifndef CORE_NODE_TREE_TRAVERSE_H_E3ACBA14_B4A0_5ECF_9FE3_EC5D970C29E4
#define CORE_NODE_TREE_TRAVERSE_H_E3ACBA14_B4A0_5ECF_9FE3_EC5D970C29E4

#include <core/node/common.h>
#include <core/node/type_constraint.h>
#include "node_tree.h"
#include "exceptions.h"

namespace rainynite::core {

enum class RecordType {
    Nothing,
    Value,
    List,
    Map
};

class TreeTraverser {
public:
    /**
     * Called when traverser enters object.
     *
     * If this function returns true, object content is traversed, otherwise not.
     */
    virtual bool object_start() = 0;
    virtual void object_end() = 0;

    void traverse_tree(shared_ptr<NodeTree> tree);

protected:
    struct Status {
        NodeTree::Index index;
        AbstractReference node;
        TypeConstraint type;
        string key;
//         size_t total_count;
//         size_t current_count;
    };

    Status const& current() const {
        return status_stack.back();
    }
    Status& current() {
        return status_stack.back();
    }
    Status const& parent() const {
        return status_stack.rbegin()[1];
    }

    vector<Status> status_stack;
    NodeTreePath path;

private:
    map<AbstractReference,size_t> node_count;
    map<AbstractReference,size_t> node_seen_count;
    bool traverse_children;
};

} // namespace rainynite::core

#endif
