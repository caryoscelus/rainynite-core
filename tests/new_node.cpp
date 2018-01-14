/*  new_node.cpp - test new node system
 *  Copyright (C) 2017-2018 caryoscelus
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
#include "zero_context.h"
#include "new_node.h"

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("New node system: simple Add node", "[node]") {
    auto add = make_shared<Add>();
    CHECK(add->value(zero_context()) == 0);
    add->set_property("a", make_value<double>(1));
    CHECK(add->value(zero_context()) == 1);
    add->set_property("b", make_value<double>(2));
    CHECK(add->value(zero_context()) == 3);
}

TEST_CASE("New node system: custom properties", "[node]") {
    auto add = make_shared<Add>();
    CHECK_THROWS_AS(add->get_property("_custom"), NodeAccessError);
    auto zero = make_value<double>(0);
    add->set_property("_custom", zero);
    CHECK(add->get_property("_custom") == zero);
    CHECK(add->get_link(add->link_count()-1) == zero);
    add->remove(add->link_count()-1);
    CHECK_THROWS_AS(add->get_property("_custom"), NodeAccessError);
}
