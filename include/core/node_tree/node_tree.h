/*  node_tree/node_tree.h - Node tree
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

#include <boost/operators.hpp>

#include <core/std/memory.h>
#include <core/std/map.h>
#include <core/std/vector.h>
#include <core/abstract_factory.h>
#include <core/util/exceptions.h>
#include <core/util/class_init.h>
#include <core/node/common.h>
#include <core/node/type_constraint.h>

namespace rainynite::core {

class NodeTree;

struct NodeTreeIndex : boost::totally_ordered<NodeTreeIndex> {
    NodeTreeIndex(size_t value_=0) :
        value(value_)
    {}
    bool operator==(NodeTreeIndex const& other) const {
        return value == other.value;
    }
    bool operator<(NodeTreeIndex const& other) const {
        return value < other.value;
    }
    NodeTreeIndex& operator++() {
        ++value;
        return *this;
    }
    NodeTreeIndex& operator--() {
        --value;
        return *this;
    }
    operator bool() const {
        return value;
    }
    operator size_t() const {
        return value;
    }
    size_t value;
};

class TreeElement {
public:
    virtual ~TreeElement() {}
    virtual void added(NodeTree const& tree, NodeTreeIndex index) = 0;
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

class NodeTree {
public:
    using Index = NodeTreeIndex;

    explicit NodeTree(shared_ptr<AbstractValue> root_);
    virtual ~NodeTree();

    Index get_root_index() const {
        return 1;
    }
    Index get_null_index() const {
        return 0;
    }
    bool is_root(Index idx) const {
        return idx == get_root_index();
    }

    Index index(Index parent, size_t i) const;
    Index parent(Index index) const;
    size_t link_index(Index index) const;
    string link_key(Index index) const;
    TypeConstraint type_of(Index index) const;
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

    observer_ptr<TreeElement> get_element(Type type, Index index) const;

    template <class T>
    observer_ptr<T> get_element(Index index) const {
        return observer_ptr<T>(
            static_cast<T*>(get_element(typeid(T), index).get())
        );
    }

    using NodeMap = map<weak_ptr<AbstractValue>, size_t, std::owner_less<weak_ptr<AbstractValue>>>;
    NodeMap const& get_node_count() const {
        return node_count;
    }

    struct Content;
    Content& get_content(Index index) const;

    void set_node_at_index(Index index, AbstractReference value);
    Index insert_index_at(Index parent, size_t position, string const& name, AbstractReference value);

    void reload_children(Index index);
    void invalidate_index(Index index);
    void invalidate_children(Index index);

private:
    Index new_index();
    void increase_node_count(weak_ptr<AbstractValue> node);
    void create_index(Index index, Index parent, size_t link_index, string link_key, TypeConstraint tye, shared_ptr<AbstractValue> node);

    void load_children(Index index, Content& element);

private:
    shared_ptr<AbstractValue> const root;

    Index last_index;

    mutable map<Index, Content> content;

    NodeMap node_count;
};

} // namespace rainynite::core

#endif
