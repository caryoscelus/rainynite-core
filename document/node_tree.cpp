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
#include <core/node_tree.h>

namespace rainynite::core {

NodeTree::NodeTree(shared_ptr<AbstractValue> root_, shared_ptr<ActionStack> action_stack_) :
    root(root_),
    action_stack(action_stack_),
    null_index(make_unique<NodeTreeIndex>(NodeTreeIndex::Null)),
    root_index(make_unique<NodeTreeIndex>(NodeTreeIndex::Root))
{
    if (root == nullptr)
        throw NullPointerException("Root of node tree cannot be null");
}

NodeTree::~NodeTree() {
}

NodeTree::Index NodeTree::index(Index parent, size_t i) const {
    switch (parent->state) {
        case NodeTreeIndex::Null:
            return get_root();
        case NodeTreeIndex::Root:
            return get_index(root_indexes, parent, i);
        case NodeTreeIndex::Indexed:
            auto parent_iter = indexes.find(parent);
            if (parent_iter == indexes.end())
                return get_null();
            return get_index(parent_iter->second, parent, i);
    }
}

NodeTree::Index NodeTree::get_index(IndexMap& indexes, Index parent, size_t i) const {
    auto iter = indexes.find(i);
    if (iter != indexes.end())
        return make_observer(iter->second.get());
    indexes.emplace(
        i,
        make_unique<NodeTreeIndex>(
            NodeTreeIndex::Indexed,
            parent,
            i
        )
    );
    return make_observer(indexes.at(i).get());
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

} // namespace rainynite::core
