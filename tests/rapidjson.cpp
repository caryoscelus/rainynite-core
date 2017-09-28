/*
 *  rapidjson.cpp - rapidjson (de)serialization tests
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

#include <boost/uuid/uuid_io.hpp>

#include <core/type_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/serialize/rapidjson.h>
#include <core/serialize/node_writer.h>
#include <core/serialize/node_reader.h>


using namespace rainynite::core;

namespace rainynite::core {

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
    double get(shared_ptr<Context> context) const override {
        auto a = get_a()->get(context);
        auto b = get_b()->get(context);
        return a+b;
    }

    NODE_PROPERTY(a, double);
    NODE_PROPERTY(b, double);
};

REGISTER_NODE(Add);

}

const char t1[] = R"({
    "UID": "af0b9a7a-1ded-11e7-bc83-5fbbc1c89c41",
    "TYPE": "Value<Real>",
    "VALUE": "4.0"
})";

const char t2[] = R"({
    "UID": "69fc1c3e-1ded-11e7-a9b3-139a4f764786",
    "TYPE": "Add",
    "a": {
        "UID": "ae861666-1ded-11e7-a77f-f7e6f7639c1d",
        "TYPE": "Value<Real>",
        "VALUE": "3.0"
    },
    "b": "ae861666-1ded-11e7-a77f-f7e6f7639c1d"
})";

struct DummyReader {};

AbstractReference parse_string(const char* s) {
    serialize::RJSONWrapper<serialize::NodeDeserializer<DummyReader>> node_reader;
    rapidjson::Reader json_reader;
    rapidjson::StringStream stream(s);
    json_reader.Parse(stream, node_reader);
    return node_reader.get_node_reader().get_root();
}

TEST_CASE("Rapidjson deserialize value", "[node]") {
    auto node = parse_string(t1);
    CHECK(node != nullptr);
    auto n = dynamic_cast<BaseValue<double>*>(node.get());
    CHECK(n->get({}) == 4.0);
}

TEST_CASE("Rapidjson deserialize add", "[node]") {
    auto node = parse_string(t2);
    CHECK(node != nullptr);
    auto n = dynamic_cast<BaseValue<double>*>(node.get());
    CHECK(n->get({}) == 6);
}
