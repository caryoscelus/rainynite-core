/*
 *  serialize.cpp - serializing tests
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

#include <iostream>

#include <core/types.h>
#include <core/serialize/json.h>
#include <core/serialize/node_writer.h>
#include <core/serialize/node_reader.h>

using namespace core;

namespace core {

namespace serialize {
template class AutoValueToString<double>;
}

class Add : public Node<double> {
public:
    Add() {
        init<double>(a, 0);
        init<double>(b, 0);
    }
public:
    virtual double get(Time t) const override {
        auto a = get_a()->get(t);
        auto b = get_b()->get(t);
        return a+b;
    }

    NODE_PROPERTY(a, double);
    NODE_PROPERTY(b, double);
};

REGISTER_NODE(Add);

}

TEST_CASE("Dumb json serialize", "[serialize,node]") {
    std::ostringstream stream;
    auto writer = serialize::DumbJsonWriter<serialize::NodeWriter, AbstractReference>(stream);
    AbstractReference three = make_value<double>(3);
    auto add = make_node<Add>();
    add->set_property("a", three);
    add->set_property("b", three);
    AbstractReference add_a = add;
    writer.object(add_a);
    std::cout << stream.str() << std::endl;
}

TEST_CASE("Read double", "[serialize]") {
    auto x = parse_primitive_type_to<double>("Real", "3.5");
    CHECK(x == 3.5);
}

class DummyReader {};

TEST_CASE("Deserialize", "[serialize,node]") {
    auto uuid_gen = boost::uuids::random_generator();
    serialize::NodeDeserializer<DummyReader> s;
    s.object_start(uuid_gen());
    s.type("Add");
    s.key("a");
    s.object_start(uuid_gen());
    s.type("Value<Real>");
    s.object_value_start();
    s.string("1");
    s.object_value_end();
    s.object_end();
    s.key("b");
    s.object_start(uuid_gen());
    s.type("Value<Real>");
    s.object_value_start();
    s.string("2");
    s.object_value_end();
    s.object_end();
    s.object_end();
    auto add = s.get_root();
    CHECK(add->get_type() == typeid(double));
    CHECK(boost::any_cast<double>(add->get_any({})) == 3);
}
