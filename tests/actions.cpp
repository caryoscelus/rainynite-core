/*
 *  actions.cpp - test actions
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

#include <catch.hpp>

#include <core/actions/change_value.h>

using namespace core;

TEST_CASE("Undo/redo value change", "[action,node]") {
    ActionStack action_stack;
    auto one = make_value<double>(1.0);
    CHECK(one->mod() == 1.0);
    action_stack.emplace<actions::ChangeValue>(one, 2.0);
    CHECK(one->mod() == 2.0);
    action_stack.emplace<actions::ChangeValue>(one, 3.0);
    CHECK(one->mod() == 3.0);
    action_stack.undo();
    CHECK(one->mod() == 2.0);
    action_stack.undo();
    CHECK(one->mod() == 1.0);
    CHECK(!action_stack.undo());
    CHECK(one->mod() == 1.0);
    action_stack.redo();
    CHECK(one->mod() == 2.0);
    action_stack.emplace<actions::ChangeValue>(one, 4.0);
    CHECK(one->mod() == 4.0);
    CHECK(!action_stack.redo());
    CHECK(one->mod() == 4.0);
}
