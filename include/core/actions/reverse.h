/*
 *  reverse.h - reverse action template
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

#ifndef CORE_ACTIONS_REVERSE_H_26C5FE47_BDF4_5E44_927E_EA8A002305F9
#define CORE_ACTIONS_REVERSE_H_26C5FE47_BDF4_5E44_927E_EA8A002305F9

namespace rainynite::core::actions {

template <class A>
class ReverseAction : public A {
public:
    template <typename... Args>
    ReverseAction(Args&&... args) :
        A(std::forward<Args>(args)...)
    {}

    void redo_action() override {
        A::undo_action();
    }
    void undo_action() override {
        A::redo_action();
    }
};

} // namespace rainynite::core::actions

#endif
