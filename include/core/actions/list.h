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
#include "reverse.h"

namespace rainynite::core::actions {

class ListPushNew : public AtomicAction {
    DOC_STRING("Push new list element")
public:
    ListPushNew(shared_ptr<AbstractListLinked> node_) :
        node(node_)
    {}

    void redo_action() override {
        node->push_new();
    }
    void undo_action() override {
        node->pop();
    }

private:
    shared_ptr<AbstractListLinked> const node;
};

/**
 * Push existing node to list.
 */
class ListPush : public AtomicAction {
    DOC_STRING("Push list element")
public:
    ListPush(shared_ptr<AbstractListLinked> node_, AbstractReference value_) :
        node(node_),
        value(value_)
    {}

    void redo_action() override {
        node->push_back(value);
    }
    void undo_action() override {
        // TODO: make pop return last link?
        value = node->get_link(node->link_count());
        node->pop();
    }

private:
    shared_ptr<AbstractListLinked> const node;
    AbstractReference value;
};

class ListInsertElement : public AtomicAction {
    DOC_STRING("Insert list element")
public:
    ListInsertElement(shared_ptr<AbstractListLinked> node_, size_t index_, AbstractReference value_) :
        node(node_),
        index(index_),
        value(value_)
    {}

    void redo_action() override {
        node->insert(index, value);
    }
    void undo_action() override {
        value = node->get_link(index);
        node->remove(index);
    }
private:
    shared_ptr<AbstractListLinked> const node;
    size_t index;
    AbstractReference value;
};

class ListRemoveElement : public ReverseAction<ListInsertElement> {
    DOC_STRING("Remove list element")
public:
    ListRemoveElement(shared_ptr<AbstractListLinked> node_, size_t index_) :
        ReverseAction<ListInsertElement>(node_, index_, nullptr)
    {}
};

} // namespace rainynite::core::actions

#endif
