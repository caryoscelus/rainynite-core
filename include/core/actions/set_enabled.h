/*  actions/set_enabled.h - enable/disable node action
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

#ifndef CORE_ACTIONS_SET_ENABLED_H_646A19C5_2CDE_5AF8_A5D6_541F296B11AC
#define CORE_ACTIONS_SET_ENABLED_H_646A19C5_2CDE_5AF8_A5D6_541F296B11AC

#include <core/action.h>
#include <core/node/abstract_value.h>

namespace rainynite::core::actions {

/**
 * Set node enabled status.
 *
 * TODO: make action disappear if it does nothing
 */
class SetEnabled : public AtomicAction {
    DOC_STRING("Set enabled")
public:
    SetEnabled(shared_ptr<AbstractValue> node_, bool value_) :
        node(node_),
        value(value_)
    {}

    void redo_action() override {
        if (value != node->enabled()) {
            node->set_enabled(value);
            value = !value;
        }
    }
    void undo_action() override {
        redo_action();
    }

private:
    shared_ptr<AbstractValue> node;
    bool value;
};

} // namespace rainynite::core::actions

#endif
