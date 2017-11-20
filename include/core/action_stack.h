/*  action_stack.h - Action undo/redo stack
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

#ifndef CORE_ACTION_STACK_H_47FDE2C6_400A_5771_817C_12BBB6192ED1
#define CORE_ACTION_STACK_H_47FDE2C6_400A_5771_817C_12BBB6192ED1

#include <boost/signals2/signal.hpp>

#include <core/std/vector.h>
#include <core/std/memory.h>
#include "action.h"

namespace rainynite::core {

class ActionStack {
public:
    using Stack = vector<unique_ptr<AbstractAction>>;
public:
    template <typename R, typename... Ts>
    void emplace(Ts&&... args) {
        push(make_unique<R>(std::forward<Ts>(args)...));
    }

    /// Add action to the stack
    void push(unique_ptr<AbstractAction> action);

    /// Try to append to last action
    bool append(AbstractAction const& action);

    /// Close latest action so that it can no longer be appended
    void close();

    bool undo();
    bool redo();

    /// Clear undo&redo stack
    void clear();

    boost::signals2::signal<void()> action_closed;
    boost::signals2::signal<void()> undone_or_redone;

private:
    bool undo_redo(Stack& from, Stack& to, UndoRedo op);
private:
    Stack undo_stack;
    Stack redo_stack;
};

} // namespace rainynite::core

#endif
