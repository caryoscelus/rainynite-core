/*
 *  action.h - Action - command pattern
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

#ifndef __CORE__ACTION_H__F4E4BABC
#define __CORE__ACTION_H__F4E4BABC

#include <stdexcept>
#include <vector>
#include <memory>

namespace core {

struct ActionError : public std::runtime_error {
    ActionError(std::string const& msg) :
        std::runtime_error(msg)
    {}
};

enum class UndoRedo {
    Undo,
    Redo
};

class AbstractAction {
public:
    void redo() {
        if (done)
            throw ActionError("Attempt to redo done action");
        redo_action();
        done = true;
    }
    void undo() {
        if (!done)
            throw ActionError("Attempt to undo undone action");
        undo_action();
        done = false;
    }
    void undo_redo(UndoRedo op) {
        if (op == UndoRedo::Undo)
            undo();
        else if (op == UndoRedo::Redo)
            redo();
        else
            throw ActionError("Unknown undo-redo op");
    }
protected:
    virtual void redo_action() = 0;
    virtual void undo_action() = 0;
private:
    bool done = false;
};

class ActionStack {
public:
    using Stack = std::vector<std::unique_ptr<AbstractAction>>;
public:
    template <typename R, typename... Ts>
    void emplace(Ts&&... args) {
        auto action = new R(std::forward<Ts>(args)...);
        action->redo();
        undo_stack.emplace_back(action);
        redo_stack.clear();
    }
    void push(std::unique_ptr<AbstractAction> action) {
        action->redo();
        undo_stack.push_back(std::move(action));
        redo_stack.clear();
    }
    bool undo() {
        return undo_redo(undo_stack, redo_stack, UndoRedo::Undo);
    }
    bool redo() {
        return undo_redo(redo_stack, undo_stack, UndoRedo::Redo);
    }
private:
    bool undo_redo(Stack& from, Stack& to, UndoRedo op) {
        if (from.size() == 0)
            return false;
        auto action = std::move(from.back());
        from.pop_back();
        action->undo_redo(op);
        to.push_back(std::move(action));
        return true;
    }
private:
    Stack undo_stack;
    Stack redo_stack;
};

} // namespace core

#endif
