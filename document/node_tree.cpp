/*  node_tree.cpp - Node tree
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

#include <core/std/vector.h>
#include <core/node/abstract_list.h>
#include <core/node/abstract_value.h>
#include <core/exceptions.h>
#include <core/document.h>
#include <core/node_tree_traverse.h>

namespace rainynite::core {

NodeTree::NodeTree(shared_ptr<AbstractValue> root_, shared_ptr<ActionStack> action_stack_) :
    root(root_),
    action_stack(action_stack_),
    null_index(NodeTreeIndex::Null),
    root_index(NodeTreeIndex::Root)
{
    if (root == nullptr)
        throw NullPointerException("Root of node tree cannot be null");
}

NodeTree::~NodeTree() {
}

NodeTree::Index NodeTree::index(Index parent, size_t i) const {
    switch (parent->state) {
        case NodeTreeIndex::Null:
            return get_root_index();
        case NodeTreeIndex::Root:
            return get_index(root_indexes, parent, i);
        case NodeTreeIndex::Indexed:
            auto parent_iter = indexes.find(parent);
            if (parent_iter == indexes.end())
                return get_null_index();
            return get_index(parent_iter->second, parent, i);
    }
}

NodeTree::Index NodeTree::get_index(IndexMap& indexes, Index parent, size_t i) const {
    auto iter = indexes.find(i);
    if (iter != indexes.end())
        return make_observer(&iter->second);
    indexes.emplace(
        i,
        NodeTreeIndex{
            NodeTreeIndex::Indexed,
            parent,
            i
        }
    );
    return make_observer(&indexes.at(i));
}

size_t NodeTree::children_count(Index parent) const {
    if (parent->null())
        return 1;
    if (auto node = get_node_as<AbstractListLinked>(parent))
        return node->link_count();
    return 0;
}

shared_ptr<AbstractValue> NodeTree::get_node(Index index) const {
    vector<size_t> indexes;
    while (!index->root()) {
        if (index->null())
            return nullptr;
        indexes.push_back(index->index);
        index = index->parent;
    }
    auto node = root;
    while (!indexes.empty()) {
        if (node == nullptr)
            return nullptr;
        size_t id = indexes.back();
        auto list = dynamic_pointer_cast<AbstractListLinked>(node);
        if (id >= list->link_count())
            return nullptr;
        node = list->get_link(id);
        indexes.pop_back();
    }
    return node;
}


void TreeTraverser::traverse_tree(shared_ptr<NodeTree> tree, TraverseFlags flags) {
    path = NodeTreePath{};
    status_stack.clear();

    status_stack.emplace_back();
    current().index = tree->get_root_index();
    current().node = tree->root_node();
    current().type = types::Any();
    current().count = 0;

    traverse_children = true;
    object_start();

    do {
        size_t i;
        if (traverse_children) {
            path.indexes.emplace_back(0);
            status_stack.emplace_back();
            i = 0;
        } else {
            i = ++path.indexes.back();
        }

        assert(status_stack.size() > 1);

        auto parent_list = list_cast(parent().node);
        if (parent_list == nullptr || i >= parent_list->link_count()) {
            path.indexes.pop_back();
            status_stack.pop_back();
            object_end();
            traverse_children = false;
        } else {
            if (auto parent_node = abstract_node_cast(parent_list)) {
                current().key = parent_node->get_name_at(i);
            } else {
                current().key.clear();
            }
            current().index = tree->index(parent().index, i);
            current().node = parent_list->get_link(i);
            current().type = parent_list->get_link_type(i);

            if (flags & UseCount) {
                auto iter = node_seen_count.find(current().node);
                if (iter == node_seen_count.end()) {
                    node_seen_count.emplace(current().node, 0);
                    current().count = 0;
                } else {
                    current().count = ++iter->second;
                }
            }

            traverse_children = object_start();
        }

    } while (!path.indexes.empty());
}

} // namespace rainynite::core
