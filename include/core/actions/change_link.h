/*  change_link.h - change child link action
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

#ifndef CORE_ACTIONS_CHANGE_LINK_H_CB8454D2_D558_5662_86E9_98499B171404
#define CORE_ACTIONS_CHANGE_LINK_H_CB8454D2_D558_5662_86E9_98499B171404

#include <core/action.h>
#include <core/node/abstract_node.h>
#include <core/node_tree/actions.h>
#include <core/node_tree/has_tree.h>

namespace rainynite::core::actions {

class ChangeLink : public AtomicAction, private HasTree {
    DOC_STRING("Change link")
public:
    ChangeLink(weak_ptr<NodeTree> tree_, NodeTree::Index index_, AbstractReference new_value_) :
        HasTree(tree_),
        index(index_),
        new_value(new_value_)
    {}

    void redo_action() override {
        old_value = tree()->get_node(index);
        replace_index(*tree(), index, new_value);
    }
    void undo_action() override {
        new_value = tree()->get_node(index);
        replace_index(*tree(), index, old_value);
    }

private:
    NodeTree::Index index;
    AbstractReference new_value;
    AbstractReference old_value;
};

/**
 * Set node's named property.
 *
 * This is essentially same as ChangeLink except that it takes AbstractNode
 * pointer and string as arguments.
 */
class SetProperty : public ChangeLink {
    DOC_STRING("Set property")
public:
    SetProperty(weak_ptr<NodeTree> tree_, NodeTree::Index parent, string const& name, AbstractReference new_value) :
        ChangeLink(
            tree_,
            index_of_property(*no_null(tree_.lock()), parent, name),
            new_value
        )
    {}
};

} // namespace rainynite::core::actions

#endif
