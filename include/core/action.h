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

#include <core/std/string.h>
#include <core/util/doc_string.h>

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
 *
 * Actions are supposed to be appendable by default, which is why append_action
 * is left abstract. If your action is not appendable, inherit from AtomicAction
 * instead.
 */
class AbstractAction : public DocString {
public:
    virtual ~AbstractAction() = default;

    void redo();
    void undo();

    /// Perform either undo or redo depending on op
    void undo_redo(UndoRedo op);

    bool append(AbstractAction const& action);

    /**
     * Close action so that it can no longer be appended
     *
     * @returns false if action was already closed
     */
    bool close();

    /// Whether this action type can ever be appended
    virtual bool appendable() {
        return true;
    }

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
    virtual bool append_action(AbstractAction const& action) = 0;

private:
    bool done = false;
    bool closed = false;
};

/**
 * Action that cannot be appended.
 *
 * This is a convenience base class with do-nothing
 * implementation.
 */
class AtomicAction : public AbstractAction {
public:
    bool appendable() override {
        return false;
    }
protected:
    bool append_action(AbstractAction const& /*action*/) override {
        return false;
    }
};

} // namespace rainynite::core

#endif
