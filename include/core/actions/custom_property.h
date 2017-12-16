/*  custom_property.h - add/remove custom property actions
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

#ifndef CORE_ACTIONS_CUSTOM_PROPERTY_H_EE0ABE90_4B56_56DB_A4C1_64CA4195A921
#define CORE_ACTIONS_CUSTOM_PROPERTY_H_EE0ABE90_4B56_56DB_A4C1_64CA4195A921

#include <core/action.h>
#include <core/node/abstract_node.h>
#include "reverse.h"

namespace rainynite::core::actions {

class AddCustomProperty : public AtomicAction {
    DOC_STRING("Add custom property")
public:
    AddCustomProperty(shared_ptr<AbstractNode> node_, string const& prop_name_, AbstractReference value_) :
        node(node_),
        prop_name(prop_name_),
        value(value_)
    {}

    void redo_action() override {
        node->set_property(prop_name, value);
    }
    void undo_action() override {
        value = node->get_property(prop_name);
        node->remove_property(prop_name);
    }
private:
    shared_ptr<AbstractNode> node;
    string prop_name;
    AbstractReference value;
};

class RemoveCustomProperty : public ReverseAction<AddCustomProperty> {
    DOC_STRING("Remove custom property")
public:
    RemoveCustomProperty(shared_ptr<AbstractNode> node_, string const& prop_name_) :
        ReverseAction<AddCustomProperty>(node_, prop_name_, nullptr)
    {}
};

} // namespace rainynite::core::actions

#endif
