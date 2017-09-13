/*
 *  node.cpp - node tests
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
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/node/traverse.h>
#include <core/context.h>

using namespace rainynite;
using namespace rainynite::core;

using Real = double;
template <typename T>
using List = vector<T>;

class Add : public Node<Real> {
public:
    Add() {
        init<Real>(a, 0);
        init<Real>(b, 0);
    }
public:
    Real get(shared_ptr<Context> context) const override {
        auto a = get_a()->get(context);
        auto b = get_b()->get(context);
        return a+b;
    }

    NODE_PROPERTY(a, Real);
    NODE_PROPERTY(b, Real);
};

shared_ptr<Context> zero_context() {
    static auto instance = std::make_shared<Context>();
    return instance;
}

TEST_CASE("Test Add node", "[node]") {
    auto add = std::make_shared<Add>();
    auto one = make_value<Real>(1.0);
    auto two = make_value<Real>(2.0);
    CHECK(add->get(zero_context()) == 0.0);
    add->set_a(one);
    CHECK(add->get(zero_context()) == 1.0);
    add->set_property("b", two);
    CHECK(add->get(zero_context()) == 3.0);
    one->set(4.0);
    CHECK(add->get(zero_context()) == 6.0);
}

class Sum : public Node<Real> {
public:
    Sum() {
        init<List<Real>>(list, {});
    }
public:
    Real get(shared_ptr<Context> context) const override {
        auto list = get_list()->get(context);
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
    CHECK(sum->get(zero_context()) == 0.0);
    sum->set_property("list", list);
    CHECK(sum->get(zero_context()) == 0.0);
    list->mod().push_back(1.0);
    CHECK(sum->get(zero_context()) == 1.0);
    list->mod().push_back(4.0);
    CHECK(sum->get(zero_context()) == 5.0);
}

class SumNode : public Node<Real> {
public:
    SumNode() {
        init<List<BaseReference<Real>>>(list, {});
    }
public:
    Real get(shared_ptr<Context> context) const override {
        auto list = get_list()->get(context);
        Real result = 0;
        for (auto x : list)
            result += x->get(context);
        return result;
    }

    NODE_PROPERTY(list, List<BaseReference<Real>>);
};

TEST_CASE("Real node sum", "[node]") {
    auto list = make_value<List<BaseReference<Real>>>();
    auto sum = std::make_shared<SumNode>();
    CHECK(sum->get(zero_context()) == 0.0);
    sum->set_property("list", list);
    CHECK(sum->get(zero_context()) == 0.0);
    auto one = make_value<Real>(1.0);
    list->mod().push_back(one);
    CHECK(sum->get(zero_context()) == 1.0);
    list->mod().push_back(one);
    CHECK(sum->get(zero_context()) == 2.0);
    one->set(3.0);
    CHECK(sum->get(zero_context()) == 6.0);
}

string value_to_string(AbstractReference node) {
    if (node->get_type() == typeid(Real)) {
        auto t = std::static_pointer_cast<Value<Real>>(node);
        return std::to_string(t->mod());
    }
    return "";
}

void serialize_map(std::ostream& stream, map<string, AbstractReference> const& named_map) {
    stream << "{\n";
    for (auto const& e : named_map) {
        stream << "\"" << e.first << "\": \"" << e.second->get_id() << "\"\n";
    }
    stream << "}\n";
}

string dump_node_tree(AbstractReference root) {
    std::ostringstream stream;
    traverse_once<bool>(root, [&stream](AbstractReference node) -> optional<bool> {
        stream << "\"" << node->get_id() << "\": ";
        if (node->is_const()) {
            stream << value_to_string(node) << "\n";
        } else if (auto linked_node = std::dynamic_pointer_cast<AbstractNode>(node)) {
            serialize_map(stream, linked_node->get_link_map());
        }
        return {};
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
    traverse_once<bool>(root, [&result](AbstractReference) -> optional<bool> {
        ++result;
        return {};
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
    SECTION("Simple") {
        auto one = make_value<Real>(1.0);
        auto two = shallow_copy(*one);
        CHECK(one->get_id() != two->get_id());
        auto one_value = dynamic_cast<Value<Real>*>(one.get());
        auto two_value = dynamic_cast<Value<Real>*>(two.get());
        two_value->set(2.0);
        CHECK(one_value->mod() == 1.0);
        CHECK(two_value->mod() == 2.0);
    }
    SECTION("Complex") {
        auto one = make_value<Real>(1.0);
        auto add = make_node_with_name<AbstractNode>("Add");
        add->set_property("a", one);
        add->set_property("b", one);
        auto add_again = shallow_copy_as<AbstractNode>(dynamic_cast<AbstractValue const&>(*add));
        add->set_property("a", shallow_copy(*one));
        add->get_property_as<double>("a")->set(2.0);
        CHECK(add->get_property_as<double>("a")->mod() == 2.0);
        CHECK(add->get_link_as<double>(0)->mod() == 2.0);
        CHECK(add->get_property_as<double>("b")->mod() == 1.0);
        CHECK(add->get_link_as<double>(1)->mod() == 1.0);
        CHECK(add_again->get_property_as<double>("a")->mod() == 1.0);
        CHECK(add_again->get_link_as<double>(0)->mod() == 1.0);
        CHECK(add_again->get_property_as<double>("b")->mod() == 1.0);
        CHECK(add_again->get_link_as<double>(1)->mod() == 1.0);
    }
}

TEST_CASE("Node children change notify", "[node]") {
    auto add = make_node_with_name<Node<double>>("Add");
    bool changed = false;
    add->subscribe([&changed](){
        changed = true;
    });
    auto one = make_value<double>(1.0);
    add->set_property("a", one);
    CHECK(changed);
    changed = false;
    one->set(2.0);
    CHECK(changed);
    changed = false;
    auto zero = add->get_property_as<double>("b");
    zero->set(1.0);
    CHECK(changed);
}

TEST_CASE("List node children change notify", "[node]") {
    auto one = make_value<double>(1.0);
    auto list = make_node<ListValue<double>>();
    bool changed = false;
    list->subscribe([&changed](){
        changed = true;
    });
    list->push_back(one);
    one->set(2.0);
    CHECK(changed);
}

TEST_CASE("Removing custom properties", "[node]") {
    auto add = make_node_with_name<AbstractNode>("Add");
    CHECK(add->link_count() == 2);
    add->set_property("_custom_0", make_value<double>(0.0));
    CHECK(add->link_count() == 3);
    add->set_property("_custom_1", make_value<double>(1.0));
    CHECK(add->link_count() == 4);
    CHECK(add->get_property_as<double>("_custom_0")->get(zero_context()) == 0);
    CHECK(add->get_property_as<double>("_custom_1")->get(zero_context()) == 1);
    add->remove_property("_custom_0");
    CHECK(add->link_count() == 3);
    CHECK(add->get_property_as<double>("_custom_1")->get(zero_context()) == 1);
}
