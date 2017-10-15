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

bool AbstractAction::close() {
    if (closed)
        return false;
    return closed = true;
}

} // namespace rainynite::core
