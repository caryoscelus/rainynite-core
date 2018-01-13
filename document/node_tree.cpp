/*  node_tree.cpp - Node tree
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

#include <core/std/vector.h>
#include <core/node/abstract_list.h>
#include <core/node/abstract_value.h>
#include <core/node/abstract_node.h>
#include <core/util/exceptions.h>
#include <core/util/nullptr.h>
#include <core/document.h>
#include <core/node_tree/traverse.h>

namespace rainynite::core {

struct NodeTree::Content {
    Content(Index parent_, size_t link_index_, string link_key_, TypeConstraint type_, weak_ptr<AbstractValue> node_) :
        parent(parent_),
        link_index(link_index_),
        link_key(link_key_),
        type(type_),
        node(node_)
    {}

    Index index(size_t i) const {
        auto node = get_node_as<AbstractListLinked>();
        if (node == nullptr) {
            if (get_node())
                throw InvalidIndexError("This node has no children");
            throw TreeCorruptedError("Node is lost");
        }
        auto link_count = node->link_count();
        if (link_count != children_count())
            throw TreeCorruptedError("Three element index count mis-match!");
        if (i >= link_count)
            throw InvalidIndexError("Index out of bounds");
        return children_indexes[i];
    }

    observer_ptr<TreeElement> add_element(Type type) {
        auto element = create_tree_element(type);
        auto r = make_observer(element.get());
        elements.emplace(type, std::move(element));
        return r;
    }

    observer_ptr<TreeElement> get_element(Type type) const {
        auto result = elements.find(type);
        if (result != elements.end())
            return make_observer(result->second.get());
        return nullptr;
    }

    shared_ptr<AbstractValue> get_node() const {
        return node.lock();
    }

    template <typename T>
    shared_ptr<T> get_node_as() const {
        return dynamic_pointer_cast<T>(get_node());
    }

    size_t children_count() const {
        return children_indexes.size();
    }

    Index parent;
    size_t link_index;
    string link_key;
    TypeConstraint type;
    vector<Index> children_indexes;

    weak_ptr<AbstractValue> node;
    map<Type, unique_ptr<TreeElement>> elements;
};


NodeTree::NodeTree(shared_ptr<AbstractValue> root_) :
    root(root_),
    last_index(get_root_index())
{
    if (root == nullptr)
        throw NullPointerException("Root of node tree cannot be null");
    create_index(get_root_index(), get_null_index(), 0, "", types::Any(), root);
}

NodeTree::~NodeTree() {
}

NodeTree::Index NodeTree::index(Index parent, size_t i) const {
    if (!parent) {
        if (i == 0)
            return 1;
        else
            throw InvalidIndexError("Try to get non-zero child of null index");
    }
    if (auto iter = content.find(parent); iter != content.end()) {
        return iter->second.index(i);
    }
    throw InvalidIndexError("Parent index is invalid");
}

NodeTree::Index NodeTree::parent(Index idx) const {
    if (!idx || is_root(idx))
        return get_null_index();
    return get_content(idx).parent;
}

size_t NodeTree::link_index(Index idx) const {
    return get_content(idx).link_index;
}

string NodeTree::link_key(Index idx) const {
    return get_content(idx).link_key;
}

TypeConstraint NodeTree::type_of(Index index) const {
    return get_content(index).type;
}

size_t NodeTree::children_count(Index parent) const {
    if (!parent)
        return 1;
    return get_content(parent).children_count();
}

shared_ptr<AbstractValue> NodeTree::get_node(Index index) const {
    return get_content(index).get_node();
}

NodeTree::Content& NodeTree::get_content(Index index) const {
    if (auto result = content.find(index); result != content.end())
        return result->second;
    throw InvalidIndexError("Cannot find content for the tree index");
}

observer_ptr<TreeElement> NodeTree::get_element(Type type, Index index) const {
    if (!index)
        return nullptr;
    auto& c = get_content(index);
    if (auto r = c.get_element(type)) {
        return r;
    }
    auto e = c.add_element(type);
    e->added(*this, index);
    return e;
}

void NodeTree::replace_index(Index index, shared_ptr<AbstractValue> node) {
    no_null(node);
    if (!index)
        throw InvalidIndexError("Cannot replace null index");
    if (index == get_root_index())
        throw InvalidIndexError("Cannot replace root node..");

    auto parent_node = no_null(get_content(parent(index)).get_node_as<AbstractListLinked>());
    parent_node->set_link(link_index(index), node);
    get_content(index).node = node;

    reload_children(index);
}

void NodeTree::reload_children(Index index) {
    invalidate_children(index);
    load_children(index, get_content(index));
}

void NodeTree::invalidate_children(Index index) {
    for (auto i : get_content(index).children_indexes) {
        invalidate_index(i);
    }
}

void NodeTree::invalidate_index(Index index) {
    invalidate_children(index);
    auto iter = node_count.find(get_content(index).node);
    if (iter == node_count.end() || !iter->second)
        throw TreeCorruptedError("No/zero count for removed node");
    if (!--iter->second)
        node_count.erase(iter);
    content.erase(index);
}

NodeTree::Index NodeTree::new_index() {
    return ++last_index;
}

void NodeTree::create_index(Index index, Index parent, size_t link_index, string link_key, TypeConstraint type, shared_ptr<AbstractValue> node) {
    auto [iter, added] = content.try_emplace(index, parent, link_index, link_key, type, node);
    if (!added)
        throw TreeCorruptedError("Trying to create index in place of old");
    auto [count_iter, _] = node_count.emplace(node, 0);
    ++count_iter->second;
    load_children(index, iter->second);
}

void NodeTree::load_children(Index parent, Content& element) {
    element.children_indexes.clear();
    if (auto list = element.get_node_as<AbstractListLinked>()) {
        auto node = abstract_node_cast(list);
        for (size_t i = 0; i < list->link_count(); ++i) {
            string key;
            if (node)
                key = node->get_name_at(i);
            auto index = new_index();
            element.children_indexes.push_back(index);
            create_index(index, parent, i, key, list->get_link_type(i), list->get_link(i));
        }
    }
}


NodeTree::Index tree_path_to_index(NodeTree const& tree, NodeTreePath const& path) {
    auto idx = tree.get_root_index();
    for (auto i : path.indexes) {
        idx = tree.index(idx, i);
    }
    return idx;
}


void TreeTraverser::traverse_tree() {
    traverse_tree_from(tree.get_root_index());
}

void TreeTraverser::traverse_tree_from(NodeTree::Index index) {
    bool traverse_children = object_start(index);
    if (traverse_children) {
        auto const& element = tree.get_content(index);
        for (auto index : element.children_indexes) {
            traverse_tree_from(index);
        }
        object_end(index);
    }
}

} // namespace rainynite::core
