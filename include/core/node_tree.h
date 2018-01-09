/*  node_tree.h - Node tree
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

#ifndef CORE_NODE_TREE_H_7E24CFE0_81EC_5F4F_836F_2CC477F61BB4
#define CORE_NODE_TREE_H_7E24CFE0_81EC_5F4F_836F_2CC477F61BB4

#include <core/std/memory.h>
#include <core/std/map.h>
#include <core/std/vector.h>
#include <core/abstract_factory.h>
#include <core/util/exceptions.h>
#include <core/util/class_init.h>
#include <core/node/common.h>

namespace rainynite::core {

class ActionStack;

class NodeTree;

struct InvalidIndexError : public std::runtime_error {
    InvalidIndexError(string const& msg) :
        std::runtime_error(msg)
    {}
};

struct NodeTreeIndex {
    enum State {
        Null,
        Root,
        Indexed
    };

    NodeTreeIndex(State state_=Null, observer_ptr<NodeTreeIndex const> parent_=nullptr, size_t index_=0) :
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

class TreeElement {
public:
    virtual ~TreeElement() {}
    virtual void added(NodeTree const& tree, observer_ptr<NodeTreeIndex const> index) = 0;
};

#define TREE_ELEMENT(T) \
    public TreeElement, \
    private class_init::Registered< \
        AbstractFactoryImpl<TreeElement,T>, \
        T, \
        AbstractFactory<TreeElement> \
    >

inline unique_ptr<TreeElement> create_tree_element(Type type) {
    return class_init::type_info<AbstractFactory<TreeElement>, unique_ptr<TreeElement>>(type);
}

class NodeTreeContent {
public:
    NodeTreeContent(weak_ptr<AbstractValue> node_ = {}) :
        node(node_)
    {}

    observer_ptr<TreeElement> add_element(Type type);
    observer_ptr<TreeElement> get_element(Type type) const;
    shared_ptr<AbstractValue> get_node() const {
        return node.lock();
    }
private:
    weak_ptr<AbstractValue> node;
    map<Type, unique_ptr<TreeElement>> elements;
};

class NodeTree {
public:
    using Index = observer_ptr<NodeTreeIndex const>;
    using IndexMap = map<size_t,NodeTreeIndex>;

    /// Disable checking indexes because it leads to huge performance drop
    static const bool DISABLE_INDEX_CHECKS = true;


    explicit NodeTree(shared_ptr<AbstractValue> root_, shared_ptr<ActionStack> action_stack_);
    virtual ~NodeTree();

    Index get_root_index() const {
        return make_observer(&root_index);
    }
    Index get_null_index() const {
        return make_observer(&null_index);
    }

    Index index(Index parent, size_t i) const;
    Index index_wo_check(Index parent, size_t i) const;
    Index parent(Index index) const {
        return index->parent;
    }
    size_t children_count(Index parent) const;

    shared_ptr<AbstractValue> root_node() const {
        return root;
    }

    NodeTreeContent& get_content(Index index) const;

    /// Get node that is pointed to by index
    shared_ptr<AbstractValue> get_node(Index index) const {
        if (index == nullptr)
            return nullptr;
        return get_content(index).get_node();
    }

    template <class T>
    shared_ptr<T> get_node_as(Index index) const {
        return dynamic_pointer_cast<T>(get_node(index));
    }

    observer_ptr<TreeElement> get_element(Type type, Index index) const;

    template <class T>
    observer_ptr<T> get_element(Index index) const {
        return observer_ptr<T>(
            static_cast<T*>(get_element(typeid(T), index).get())
        );
    }

    void check_index_validity(Index index) const;

    void rebuild_count();

    map<AbstractReference, size_t> const& get_node_count() const {
        return node_count;
    }

    map<AbstractReference, size_t>& mod_node_count() {
        return node_count;
    }

private:
    Index get_index(IndexMap& indexes, Index parent, size_t i) const;

private:
    shared_ptr<AbstractValue> const root;
    shared_ptr<ActionStack> action_stack;
    NodeTreeIndex null_index;
    NodeTreeIndex root_index;
    mutable IndexMap root_indexes;
    mutable map<Index, IndexMap> indexes;
    mutable map<Index, NodeTreeContent> content;
    mutable map<AbstractReference, size_t> node_count;
};

NodeTree::Index tree_path_to_index(NodeTree const& tree, NodeTreePath const& path);

} // namespace rainynite::core

#endif
