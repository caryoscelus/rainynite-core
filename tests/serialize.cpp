/*  serialize.cpp - serializing tests
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

#include <iostream>

#include <core/type_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/serialize/node_reader.h>

#include "new_node.h"

REGISTER_NODE(Add);

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("Read double", "[serialize]") {
    auto x = parse_primitive_type_to<double>("Real", "3.5");
    CHECK(x == 3.5);
}

class DummyReader {};

TEST_CASE("Deserialize", "[serialize,node]") {
    auto uuid_gen = boost::uuids::random_generator();
    serialize::NodeDeserializer<DummyReader> s;
    s.object_start(uuid_gen());
    s.type("Add/Real");
    s.key("a");
    s.object_start(uuid_gen());
    s.type("Value/Real");
    s.object_value_start();
    s.value_string("1");
    s.object_value_end();
    s.object_end();
    s.key("b");
    s.object_start(uuid_gen());
    s.type("Value/Real");
    s.object_value_start();
    s.value_string("2");
    s.object_value_end();
    s.object_end();
    s.object_end();
    auto add = s.get_root();
    CHECK(add->get_type() == typeid(double));
    CHECK(any_cast<double>(add->get_any({})) == 3);
}
