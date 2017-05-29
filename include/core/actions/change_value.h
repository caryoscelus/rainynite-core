/*
 *  change_value.h - change value action
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

#ifndef __CORE__ACTIONS__CHANGE_VALUE_H__20F972B6
#define __CORE__ACTIONS__CHANGE_VALUE_H__20F972B6

#include <core/action.h>
#include <core/node/abstract_value.h>

namespace core {
namespace actions {

class ChangeValue : public AbstractAction {
public:
    ChangeValue(AbstractReference node_, boost::any new_value_) :
        node(node_),
        new_value(new_value_)
    {}
public:
    virtual void redo_action() override {
        old_value = node->any();
        node->set_any(new_value);
    }
    virtual void undo_action() override {
        new_value = node->any();
        node->set_any(old_value);
    }
private:
    AbstractReference node;
    boost::any new_value;
    boost::any old_value;
};

} // namespace actions
} // namespace core

#endif
