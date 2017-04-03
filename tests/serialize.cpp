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

#include <boost/uuid/uuid_io.hpp>

#include <core/serialize.h>
#include <core/node.h>
#include <core/node_info.h>
#include <core/class_init.h>

using namespace core;

namespace core {

class ValueToString {
public:
    virtual std::string operator()(boost::any const& object) const = 0;
};

template <class T>
class AutoValueToString : public ValueToString, class_init::Registered<AutoValueToString<T>, T, ValueToString> {
public:
    virtual std::string operator()(boost::any const& object) const override {
        auto value = boost::any_cast<T>(object);
        return std::to_string(value);
    }
};

template class AutoValueToString<double>;

class DoubleTypeName : public TypeName, class_init::Registered<DoubleTypeName, double, TypeName> {
public:
    virtual std::string operator()() const override {
        return "double";
    }
};

class ValueTypeNameBase {
public:
    virtual std::string operator()(boost::any const& object) const = 0;
};

class ValueTypeName : public ValueTypeNameBase, class_init::Registered<ValueTypeName, AbstractReference, ValueTypeNameBase> {
public:
    virtual std::string operator()(boost::any const& object) const {
        auto value = boost::any_cast<AbstractReference>(object);
        return class_init::type_info<TypeName, std::string>(value->get_type());
    }
};

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

template <typename T>
class ValueNodeInfo : public NodeInfo, class_init::Registered<ValueNodeInfo<T>, Value<T>, NodeInfo> {
public:
    virtual std::string operator()() const override {
        return "Value<"+class_init::type_info<TypeName,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return make_node<Value<T>>();
    }
};

template class ValueNodeInfo<double>;

namespace serialize {

template <>
void put_value(Writer<AbstractReference>& writer, AbstractReference const& object) {
    writer.string(class_init::any_info<ValueToString, std::string>(object->any()));
}

template <>
void put_list(Writer<AbstractReference>& writer, AbstractReference const& object) {
    //
}

template <>
void put_map(Writer<AbstractReference>& writer, AbstractReference const& object) {
    if (auto node = std::dynamic_pointer_cast<AbstractNode>(object)) {
        for (auto const& e : node->get_link_map()) {
            writer.key(e.first);
            writer.object(e.second);
        }
    }
}

template <>
AbstractReference get_reference(AbstractReference const& object) {
    return object;
}

template <>
std::string id_to_string(AbstractReference id) {
    return to_string(id->get_id());
}

template <>
std::string get_type(AbstractReference const& object) {
    return node_name(object);
}

template <>
RecordType classify(AbstractReference const& object) {
    if (object->is_const())
        return RecordType::Value;
    return RecordType::Map;
}

}
}

TEST_CASE("Dumb json serialize", "[serialize,node]") {
    std::ostringstream stream;
    auto writer = serialize::DumbJsonWriter<AbstractReference>(stream);
    AbstractReference three = make_value<double>(3);
    auto add = make_node<Add>();
    add->set_property("a", three);
    add->set_property("b", three);
    AbstractReference add_a = add;
    writer.object(add_a);
    std::cout << stream.str() << std::endl;
}
