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
#include <core/node/abstract_node.h>
#include "reverse.h"

namespace rainynite::core::actions {

class ListPushNew : public AbstractAction {
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

} // namespace rainynite::core::actions

#endif