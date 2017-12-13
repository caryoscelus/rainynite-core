/*  node_tree.h - Node tree
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

#ifndef CORE_NODE_TREE_H_7E24CFE0_81EC_5F4F_836F_2CC477F61BB4
#define CORE_NODE_TREE_H_7E24CFE0_81EC_5F4F_836F_2CC477F61BB4

#include <core/std/memory.h>
#include <core/std/map.h>
#include <core/std/vector.h>

namespace rainynite::core {

class AbstractValue;
class ActionStack;

struct NodeTreeIndex {
    enum State {
        Null,
        Root,
        Indexed
    };

    NodeTreeIndex(State state_, observer_ptr<NodeTreeIndex const> parent_=nullptr, size_t index_=0) :
        state(state_),
        parent(parent_),
        index(index_)
    {
    }

    bool null() const {
        return state == Null;
    }
    bool root() const {
        return state == Root;
    }

    State state;
    observer_ptr<NodeTreeIndex const> parent;
    size_t index;
};

struct NodeTreePath {
    template <typename... Ts>
    NodeTreePath(Ts&&... args) :
        indexes(std::forward<Ts>(args)...)
    {
    }
    vector<size_t> indexes;
};

class NodeTree {
public:
    using Index = observer_ptr<NodeTreeIndex const>;
    using IndexMap = map<size_t,NodeTreeIndex>;

    explicit NodeTree(shared_ptr<AbstractValue> root_, shared_ptr<ActionStack> action_stack_);
    virtual ~NodeTree();

    Index get_root_index() const {
        return make_observer(&root_index);
    }
    Index get_null_index() const {
        return make_observer(&null_index);
    }

    Index index(Index parent, size_t i) const;
    Index parent(Index index) const {
        return index->parent;
    }
    size_t children_count(Index parent) const;

    shared_ptr<AbstractValue> root_node() const {
        return root;
    }

    /// Get node that is pointed to by index
    shared_ptr<AbstractValue> get_node(Index index) const;

    template <class T>
    shared_ptr<T> get_node_as(Index index) const {
        return dynamic_pointer_cast<T>(get_node(index));
    }

private:
    Index get_index(IndexMap& indexes, Index parent, size_t i) const;

private:
    shared_ptr<AbstractValue> const root;
    shared_ptr<ActionStack> action_stack;
    NodeTreeIndex null_index;
    NodeTreeIndex root_index;
    mutable IndexMap root_indexes;
    mutable map<Index,IndexMap> indexes;
};

} // namespace rainynite::core

#endif
