/*  state_machine.h - state machine template
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

#ifndef CORE_STATE_MACHINE_H_F1A307CC_D2C3_5BC5_A034_8A79562E8FFF
#define CORE_STATE_MACHINE_H_F1A307CC_D2C3_5BC5_A034_8A79562E8FFF

namespace rainynite {

template <class State>
class StateMachine {
protected:
    State state() const {
        return state_stack.back();
    }
    State state(unsigned n) {
        return state_stack[state_stack.size()-(n+1)];
    }
    void push_state(State s) {
        state_stack.push_back(s);
    }
    void pop_state(State s) {
        if (state() != s)
            state_mismatch_error(s);
        state_stack.pop_back();
    }
    void set_state(State s) {
        state_stack.back() = s;
    }
    virtual void state_mismatch_error(State s) const = 0;
private:
    vector<State> state_stack;
};

} // namespace rainynite

#endif
