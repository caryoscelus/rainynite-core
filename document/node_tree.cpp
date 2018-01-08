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
#include <core/document.h>
#include <core/node_tree_traverse.h>

namespace rainynite::core {

observer_ptr<TreeElement> NodeTreeContent::add_element(Type type) {
    auto element = create_tree_element(type);
    auto r = make_observer(element.get());
    elements.emplace(type, std::move(element));
    return r;
}

observer_ptr<TreeElement> NodeTreeContent::get_element(Type type) const {
    auto result = elements.find(type);
    if (result != elements.end())
        return make_observer(result->second.get());
    return nullptr;
}


NodeTree::NodeTree(shared_ptr<AbstractValue> root_, shared_ptr<ActionStack> action_stack_) :
    root(root_),
    action_stack(action_stack_),
    null_index(NodeTreeIndex::Null),
    root_index(NodeTreeIndex::Root, get_null_index())
{
    if (root == nullptr)
        throw NullPointerException("Root of node tree cannot be null");
    content.emplace(get_root_index(), NodeTreeContent{root});
}

NodeTree::~NodeTree() {
}

NodeTree::Index NodeTree::index(Index parent, size_t i) const {
    check_index_validity(parent);
    auto idx = index_wo_check(parent, i);
    check_index_validity(idx);
    return idx;
}

NodeTree::Index NodeTree::index_wo_check(Index parent, size_t i) const {
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

NodeTree::Index NodeTree::get_index(IndexMap& local_indexes, Index parent, size_t i) const {
    auto iter = local_indexes.find(i);
    if (iter != local_indexes.end()) {
        auto r = make_observer(&iter->second);
        try {
            check_index_validity(r);
            return r;
        } catch (...) {
        }
    } else {
        local_indexes.emplace(
            i,
            NodeTreeIndex{
                NodeTreeIndex::Indexed,
                parent,
                i
            }
        );
    }
    auto index = make_observer(&local_indexes.at(i));
    indexes.emplace(index, IndexMap{});
    content[index] = NodeTreeContent{get_node_as<AbstractListLinked>(parent)->get_link(i)};
    return index;
}

size_t NodeTree::children_count(Index parent) const {
    if (parent->null())
        return 1;
    check_index_validity(parent);
    if (auto node = get_node_as<AbstractListLinked>(parent))
        return node->link_count();
    return 0;
}

NodeTreeContent& NodeTree::get_content(Index index) const {
    if (index == nullptr)
        throw NullPointerException("Null index");
    check_index_validity(index);
    auto const& result = content.find(index);
    if (result == content.end())
        throw InvalidIndexError("Cannot find content for the tree index");
    return result->second;
}

observer_ptr<TreeElement> NodeTree::get_element(Type type, Index index) const {
    if (index == nullptr)
        return nullptr;
    auto& c = get_content(index);
    if (auto r = c.get_element(type)) {
        return r;
    }
    auto e = c.add_element(type);
    e->added(*this, index);
    return e;
}

void NodeTree::check_index_validity(Index index) const {
    if (auto p_idx = parent(index)) {
        if (p_idx->null())
            return;
        check_index_validity(p_idx);
        if (auto p_node = get_node_as<AbstractListLinked>(p_idx)) {
            if (index->index < p_node->link_count()) {
                if (content.at(index).get_node() != p_node->get_link(index->index))
                    throw InvalidIndexError("Index node mis-match");
                else
                    return;
            }
        }
        throw InvalidIndexError("Index out of range");
    }
}


NodeTree::Index tree_path_to_index(NodeTree const& tree, NodeTreePath const& path) {
    auto idx = tree.get_root_index();
    for (auto i : path.indexes) {
        idx = tree.index(idx, i);
    }
    return idx;
}


class CountTraverser : public TreeTraverser {
public:
    CountTraverser(NodeTree& tree) :
        TreeTraverser(tree)
    {}

    bool object_start() {
        if (current().count > 0)
            return false;
        return true;
    }
    void object_end() {
        if (path.indexes.empty()) {
            tree.mod_node_count() = node_seen_count;
            for (auto& e : tree.mod_node_count()) {
                ++e.second;
            }
        }
    }
};

void NodeTree::rebuild_count() {
    CountTraverser traverser(*this);
    traverser.traverse_tree(TreeTraverser::UseCount);
}


void TreeTraverser::traverse_tree(TraverseFlags flags) {
    path = NodeTreePath{};
    status_stack.clear();

    status_stack.emplace_back();
    current().index = tree.get_root_index();
    current().node = tree.root_node();
    current().type = types::Any();
    current().count = 0;

    node_seen_count[current().node] = 0;

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
            current().index = tree.index(parent().index, i);
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
