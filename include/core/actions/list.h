/*  list.h - list-related actions
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

#ifndef CORE_ACTIONS_LIST_H_E2B8B263_008B_58EA_8917_059B73FCC6C0
#define CORE_ACTIONS_LIST_H_E2B8B263_008B_58EA_8917_059B73FCC6C0

#include <core/action.h>
#include <core/node/abstract_list.h>
#include <core/node_tree/actions.h>
#include <core/node_tree/has_tree.h>
#include <core/node_tree/path.h>
#include "reverse.h"

namespace rainynite::core::actions {

class ListPushNew : public AtomicAction, private HasTree {
    DOC_STRING("Push new list element")
public:
    ListPushNew(TreePtr tree_, NodeTree::Index index_) :
        HasTree(tree_),
        path(tree_index_to_path(*no_null(tree_.lock()), index_))
    {}

    void redo_action() override {
        push_new_to(*tree(), path_to_index(path));
    }
    void undo_action() override {
        pop_from(*tree(), path_to_index(path));
    }

private:
    NodeTreePath path;
};

/**
 * Push existing node to list.
 */
class ListPush : public AtomicAction, private HasTree {
    DOC_STRING("Push list element")
public:
    ListPush(TreePtr tree_, NodeTree::Index index_, AbstractReference value_) :
        HasTree(tree_),
        path(tree_index_to_path(*no_null(tree_.lock()), index_)),
        value(value_)
    {}

    void redo_action() override {
        push_to(*tree(), path_to_index(path), value);
    }
    void undo_action() override {
        value = pop_from(*tree(), path_to_index(path));
    }

private:
    NodeTreePath path;
    AbstractReference value;
};


class ListClear : public AtomicAction, public HasTree {
    DOC_STRING("Clear list")

public:
    ListClear(TreePtr tree_, Index idx) :
        HasTree(tree_),
        path(tree_index_to_path(*no_null(tree_.lock()), idx))
    {}

    void redo_action() override {
        if (auto list = tree()->get_node_as<AbstractListLinked>(path_to_index(path))) {
            if (!list->is_editable_list())
                return;
            old_children = list->get_links();
            list->clear_links();
        }
    }
    void undo_action() override {
        if (auto list = tree()->get_node_as<AbstractListLinked>(path_to_index(path))) {
            // TODO
            for (auto e : old_children) {
                list->push_back(std::move(e));
            }
        }
        old_children.clear();
    }

private:
    NodeTreePath path;
    vector<AbstractReference> old_children;
};


class ListInsertElement : public AtomicAction, protected HasTree {
    DOC_STRING("Insert list element")
public:
    ListInsertElement(TreePtr tree_, Index index_, size_t link_index_, AbstractReference value_) :
        HasTree(tree_),
        path(tree_index_to_path(*no_null(tree_.lock()), index_)),
        link_index(link_index_),
        value(value_)
    {}

    void redo_action() override {
        insert_to(*tree(), path_to_index(path), link_index, value);
    }
    void undo_action() override {
        auto child = tree()->index(path_to_index(path), link_index);
        value = tree()->get_node(child);
        remove_index(*tree(), child);
    }
private:
    NodeTreePath path;
    size_t link_index;
    AbstractReference value;
};

class ListRemoveElement : public ReverseAction<ListInsertElement> {
    DOC_STRING("Remove list element")
public:
    ListRemoveElement(TreePtr tree_, Index index_, size_t link_index_) :
        ReverseAction<ListInsertElement>(tree_, index_, link_index_, nullptr)
    {}
};

} // namespace rainynite::core::actions

#endif
