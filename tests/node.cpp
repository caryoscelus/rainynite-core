/*
 *  time.cpp - time tests
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

#include <core/node_info.h>

using namespace core;

using Real = double;
template <typename T>
using List = std::vector<T>;

class Add : public Node<Real> {
public:
    Add() {
        init<Real>(a, 0);
        init<Real>(b, 0);
    }
public:
    virtual Real get(Time t) const override {
        auto a = get_a()->get(t);
        auto b = get_b()->get(t);
        return a+b;
    }

    NODE_PROPERTY(a, Real);
    NODE_PROPERTY(b, Real);
};

TEST_CASE("Test Add node", "[node]") {
    auto add = std::make_shared<Add>();
    auto one = make_value<Real>(1.0);
    auto two = make_value<Real>(2.0);
    CHECK(add->get(Time()) == 0.0);
    add->set_a(one);
    CHECK(add->get(Time()) == 1.0);
    add->set_property("b", two);
    CHECK(add->get(Time()) == 3.0);
    one->set(4.0);
    CHECK(add->get(Time()) == 6.0);
}

class Sum : public Node<Real> {
public:
    Sum() {
        init<List<Real>>(list, {});
    }
public:
    virtual Real get(Time t) const override {
        auto list = get_list()->get(t);
        Real result = 0;
        for (auto const& x : list)
            result += x;
        return result;
    }

    NODE_PROPERTY(list, List<Real>);
};

TEST_CASE("Sum Node", "[node]") {
    auto list = make_value<List<Real>>();
    auto sum = std::make_shared<Sum>();
    CHECK(sum->get(Time()) == 0.0);
    sum->set_property("list", list);
    CHECK(sum->get(Time()) == 0.0);
    list->mod().push_back(1.0);
    CHECK(sum->get(Time()) == 1.0);
    list->mod().push_back(4.0);
    CHECK(sum->get(Time()) == 5.0);
}

class SumNode : public Node<Real> {
public:
    SumNode() {
        init<List<BaseReference<Real>>>(list, {});
    }
public:
    virtual Real get(Time t) const override {
        auto list = get_list()->get(t);
        Real result = 0;
        for (auto x : list)
            result += x->get(t);
        return result;
    }

    NODE_PROPERTY(list, List<BaseReference<Real>>);
};

TEST_CASE("Real node sum", "[node]") {
    auto list = make_value<List<BaseReference<Real>>>();
    auto sum = std::make_shared<SumNode>();
    CHECK(sum->get(Time()) == 0.0);
    sum->set_property("list", list);
    CHECK(sum->get(Time()) == 0.0);
    auto one = make_value<Real>(1.0);
    list->mod().push_back(one);
    CHECK(sum->get(Time()) == 1.0);
    list->mod().push_back(one);
    CHECK(sum->get(Time()) == 2.0);
    one->set(3.0);
    CHECK(sum->get(Time()) == 6.0);
}

std::string value_to_string(AbstractReference node) {
    if (node->get_type() == typeid(Real)) {
        auto t = std::static_pointer_cast<Value<Real>>(node);
        return std::to_string(t->mod());
    }
    return "";
}

void serialize_map(std::ostream& stream, std::map<std::string, AbstractReference> const& map) {
    stream << "{\n";
    for (auto const& e : map) {
        stream << "\"" << e.first << "\": \"" << e.second->get_id() << "\"\n";
    }
    stream << "}\n";
}

std::string dump_node_tree(AbstractReference root) {
    std::ostringstream stream;
    traverse_once<bool>(root, [&stream](AbstractReference node) {
        stream << "\"" << node->get_id() << "\": ";
        if (node->is_const()) {
            stream << value_to_string(node) << "\n";
        } else if (auto linked_node = std::dynamic_pointer_cast<AbstractNode>(node)) {
            serialize_map(stream, linked_node->get_link_map());
        }
        return boost::none;
    });
    stream << "\n";
    return stream.str();
}

TEST_CASE("Dump node tree", "[node]") {
    std::cerr << dump_node_tree(make_value<Real>(5.4));
    auto one = make_value<Real>(1.0);
    std::cerr << dump_node_tree(one);
    auto add = std::make_shared<Add>();
    std::cerr << dump_node_tree(add);
    add->set_a(one);
    std::cerr << dump_node_tree(add);
    add->set_property("b", one);
    std::cerr << dump_node_tree(add);
}

unsigned count_nodes(AbstractReference root, TraverseDepth depth = TraverseDepth::Once) {
    unsigned result = 0;
    traverse_once<bool>(root, [&result](AbstractReference) {
        ++result;
        return boost::none;
    }, depth);
    return result;
}

TEST_CASE("Traverse node tree", "[node]") {
    auto one = make_value<Real>(1.0);
    CHECK(count_nodes(one) == 1);
    auto add = std::make_shared<Add>();
    CHECK(count_nodes(add) == 3);
    add->set_a(one);
    CHECK(count_nodes(add) == 3);
    add->set_property("b", one);
    CHECK(count_nodes(add) == 2);
    CHECK(count_nodes(add, TraverseDepth::Deeper) == 3);
}

TEST_CASE("Traverse list node", "[node]") {
    auto list = make_node<ListValue<Real>>();
    auto one = make_value<Real>(1.0);
    list->push_back(one);
    CHECK(count_nodes(list) == 2);
    list->push_back(one);
    CHECK(count_nodes(list) == 2);
    CHECK(count_nodes(list, TraverseDepth::Deeper) == 3);
}

TEST_CASE("Shallow node copy", "[node]") {
    auto one = make_value<Real>(1.0);
    auto two = shallow_copy(*one);
    auto one_value = dynamic_cast<Value<Real>*>(one.get());
    auto two_value = dynamic_cast<Value<Real>*>(two.get());
    two_value->set(2.0);
    CHECK(one_value->mod() == 1.0);
    CHECK(two_value->mod() == 2.0);
}
