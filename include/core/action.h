/*  action.h - Action - command pattern (undo/redo)
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

#ifndef CORE_ACTION_H_8B0DA2F0_CBAB_5765_994D_8CE9E17977D3
#define CORE_ACTION_H_8B0DA2F0_CBAB_5765_994D_8CE9E17977D3

#include <stdexcept>
#include <core/std/vector.h>
#include <core/std/memory.h>
#include <core/std/string.h>

namespace rainynite::core {

struct ActionError : public std::runtime_error {
    ActionError(string const& msg) :
        std::runtime_error(msg)
    {}
};

enum class UndoRedo {
    Undo,
    Redo
};

/**
 * Command pattern: action.
 *
 * Can be done, undone, redone and possibly appended.
 */
class AbstractAction {
public:
    void redo();
    void undo();

    /// Perform either undo or redo depending on op
    void undo_redo(UndoRedo op);

    bool append(AbstractAction const& action);

    /// Close action so that it can no longer be appended
    void close();

protected:
    virtual void redo_action() = 0;
    virtual void undo_action() = 0;

    /**
     * Append to this action.
     *
     * Override this to support merging similar actions into one.
     *
     * @returns true if action was appended.
     */
    virtual bool append_action(AbstractAction const& /*action*/) {
        return false;
    }

private:
    bool done = false;
    bool closed = false;
};

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
private:
    bool undo_redo(Stack& from, Stack& to, UndoRedo op);
private:
    Stack undo_stack;
    Stack redo_stack;
};

} // namespace rainynite::core

#endif
