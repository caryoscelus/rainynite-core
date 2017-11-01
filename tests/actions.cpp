/*  actions.cpp - test actions
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

#include <core/node/make.h>
#include <core/node/node.h>
#include <core/node_info.h>
#include <core/action_stack.h>
#include <core/actions/change_value.h>
#include <core/actions/change_link.h>
#include <core/actions/custom_property.h>
#include <core/actions/list.h>
#include "zero_context.h"

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("Undo/redo value change", "[action,node]") {
    ActionStack action_stack;
    auto one = make_value<double>(1.0);
    CHECK(one->mod() == 1.0);
    SECTION("Simple undo/redo") {
        action_stack.emplace<actions::ChangeValue>(one, 2.0);
        action_stack.close();
        CHECK(one->mod() == 2.0);
        action_stack.emplace<actions::ChangeValue>(one, 3.0);
        action_stack.close();
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
    SECTION("Append value change") {
        action_stack.emplace<actions::ChangeValue>(one, 2.0);
        CHECK(one->mod() == 2.0);
        action_stack.emplace<actions::ChangeValue>(one, 3.0);
        CHECK(one->mod() == 3.0);
        action_stack.undo();
        CHECK(one->mod() == 1.0);
        action_stack.redo();
        CHECK(one->mod() == 3.0);
    }
}

TEST_CASE("Change link", "[action,node]") {
    ActionStack action_stack;
    auto add = make_node_with_name<BaseValue<double>>("Add/Real");
    auto add_node = dynamic_pointer_cast<AbstractListLinked>(add);
    REQUIRE(add->value(zero_context()) == 0);
    action_stack.emplace<actions::ChangeLink>(add_node, 0, make_value<double>(1));
    CHECK(add->value(zero_context()) == 1);
    action_stack.emplace<actions::ChangeLink>(add_node, 1, make_value<double>(2));
    CHECK(add->value(zero_context()) == 3);
    action_stack.undo();
    CHECK(add->value(zero_context()) == 1);
    action_stack.undo();
    CHECK(add->value(zero_context()) == 0);
}

TEST_CASE("Custom property", "[action,node]") {
    ActionStack action_stack;
    auto add = make_node_with_name<AbstractNode>("Add/Real");
    auto other_node = make_value<double>(1);
    CHECK_THROWS_AS(add->get_property("_something"), NodeAccessError);
    action_stack.emplace<actions::AddCustomProperty>(add, "_something", other_node);
    CHECK(add->get_property("_something") == other_node);
    action_stack.emplace<actions::RemoveCustomProperty>(add, "_something");
    CHECK_THROWS_AS(add->get_property("_something"), NodeAccessError);
    action_stack.undo();
    CHECK(add->get_property("_something") == other_node);
    action_stack.undo();
    CHECK_THROWS_AS(add->get_property("_something"), NodeAccessError);
}

TEST_CASE("List", "[action,node]") {
    ActionStack action_stack;
    auto list = make_shared<ListValue<double>>();
    REQUIRE(list->value(zero_context()) == vector<double>{});

    action_stack.emplace<actions::ListPushNew>(list);
    CHECK(list->value(zero_context()) == vector<double>{0});

    action_stack.emplace<actions::ListInsertElement>(list, 1, make_value<double>(1));
    CHECK(list->value(zero_context()) == (vector<double>{0, 1}));

    action_stack.emplace<actions::ListRemoveElement>(list, 0);
    CHECK(list->value(zero_context()) == vector<double>{1});

    action_stack.undo();
    action_stack.undo();
    action_stack.undo();
    CHECK(list->value(zero_context()) == vector<double>{});

    action_stack.redo();
    CHECK(list->value(zero_context()) == vector<double>{0});

    action_stack.redo();
    CHECK(list->value(zero_context()) == (vector<double>{0, 1}));

    action_stack.redo();
    CHECK(list->value(zero_context()) == vector<double>{1});
}
