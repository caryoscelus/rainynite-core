/*  action_stack.cpp - Action undo/redo stack
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

#include <core/action_stack.h>

namespace rainynite::core {

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
        bool should_close = !action->appendable();
        undo_stack.emplace_back(std::move(action));
        if (should_close)
            close();
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
    undone_or_redone();
    return true;
}

void ActionStack::close() {
    if (!undo_stack.empty()) {
        if (undo_stack.back()->close())
            action_closed();
    }
}

} // namespace rainynite::core
