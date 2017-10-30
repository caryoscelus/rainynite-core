/*  change_link.h - change child link action
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

#ifndef CORE_ACTIONS_CHANGE_LINK_H_CB8454D2_D558_5662_86E9_98499B171404
#define CORE_ACTIONS_CHANGE_LINK_H_CB8454D2_D558_5662_86E9_98499B171404

#include <core/action.h>
#include <core/node/abstract_node.h>

namespace rainynite::core::actions {

class ChangeLink : public AtomicAction {
public:
    ChangeLink(shared_ptr<AbstractListLinked> node_, size_t index_, AbstractReference new_value_) :
        node(node_),
        index(index_),
        new_value(new_value_)
    {}
public:
    void redo_action() override {
        old_value = node->get_link(index);
        node->set_link(index, new_value);
    }
    void undo_action() override {
        new_value = node->get_link(index);
        node->set_link(index, old_value);
    }
private:
    shared_ptr<AbstractListLinked> node;
    size_t index;
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
public:
    SetProperty(shared_ptr<AbstractNode> node, string const& name, AbstractReference new_value) :
        ChangeLink(dynamic_pointer_cast<AbstractListLinked>(node), node->get_name_id(name), new_value)
    {}
};

} // namespace rainynite::core::actions

#endif
