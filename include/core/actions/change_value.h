/*  change_value.h - change value action
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

#ifndef CORE_ACTIONS_CHANGE_VALUE_H_FA028A83_DD3B_5568_A0FA_F0052D8F88AD
#define CORE_ACTIONS_CHANGE_VALUE_H_FA028A83_DD3B_5568_A0FA_F0052D8F88AD

#include <core/action.h>
#include <core/node/abstract_value.h>

namespace rainynite::core::actions {

class ChangeValue : public AbstractAction {
    DOC_STRING("Change value")
public:
    ChangeValue(AbstractReference node_, any new_value_) :
        node(node_),
        new_value(new_value_)
    {}

    void redo_action() override {
        old_value = node->static_any();
        node->set_any(new_value);
    }
    void undo_action() override {
        new_value = node->static_any();
        node->set_any(old_value);
    }
protected:
    bool append_action(AbstractAction const& action) override {
        if (auto cv_action = dynamic_cast<ChangeValue const*>(&action)) {
            new_value = cv_action->new_value;
            return true;
        }
        return false;
    }
private:
    AbstractReference node;
    any new_value;
    any old_value;
};

} // namespace rainynite::core::actions

#endif
