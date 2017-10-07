/*  action.cpp - Action - command pattern (undo/redo)
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

#include <core/action.h>

namespace rainynite::core {

void AbstractAction::redo() {
    if (done)
        throw ActionError("Attempt to redo done action");
    redo_action();
    done = true;
}

void AbstractAction::undo() {
    if (!done)
        throw ActionError("Attempt to undo undone action");
    undo_action();
    done = false;
}

void AbstractAction::undo_redo(UndoRedo op) {
    if (op == UndoRedo::Undo)
        undo();
    else if (op == UndoRedo::Redo)
        redo();
    else
        throw ActionError("Unknown undo-redo op");
}

bool AbstractAction::append(AbstractAction const& action) {
    return !closed && append_action(action);
}

void AbstractAction::close() {
    closed = true;
}

bool ActionStack::append(AbstractAction const& action) {
    if (undo_stack.empty())
        return false;
    return undo_stack.back()->append(action);
}

void ActionStack::push(unique_ptr<AbstractAction> action) {
    action->redo();
    redo_stack.clear();
    if (!append(*action)) {
        close();
        undo_stack.emplace_back(std::move(action));
    }
}

bool ActionStack::undo() {
    return undo_redo(undo_stack, redo_stack, UndoRedo::Undo);
}

bool ActionStack::redo() {
    return undo_redo(redo_stack, undo_stack, UndoRedo::Redo);
}

bool ActionStack::undo_redo(Stack& from, Stack& to, UndoRedo op) {
    if (from.size() == 0)
        return false;
    auto action = std::move(from.back());
    from.pop_back();
    action->undo_redo(op);
    to.push_back(std::move(action));
    return true;
}

void ActionStack::close() {
    if (!undo_stack.empty())
        undo_stack.back()->close();
}

} // namespace rainynite::core
