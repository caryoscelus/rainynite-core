/*
 *  change_link.h - change child link action
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

#ifndef __CORE__ACTIONS__CHANGE_LINK_H__720E8E3C
#define __CORE__ACTIONS__CHANGE_LINK_H__720E8E3C

#include <core/action.h>
#include <core/node/abstract_node.h>

namespace core {
namespace actions {

class ChangeLink : public AbstractAction {
public:
    ChangeLink(std::shared_ptr<AbstractListLinked> node_, size_t index_, AbstractReference new_value_) :
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
    std::shared_ptr<AbstractListLinked> node;
    size_t index;
    AbstractReference new_value;
    AbstractReference old_value;
};

} // namespace actions
} // namespace core

#endif
