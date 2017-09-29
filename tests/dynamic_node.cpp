/*  dynamic_node.cpp - test dynamic nodes
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/context.h>

using namespace rainynite;
using namespace rainynite::core;

shared_ptr<Context> zero_context() {
    static auto instance = make_shared<Context>();
    return instance;
}

TEST_CASE("Test dynamic node", "[node]") {
    auto dynamic = make_node_with_name<Node<double>>("DynamicNode<Real>");
    dynamic->get_property("node_type")->set_any(string("Add"));
    auto list = dynamic_cast<UntypedListValue*>(dynamic->get_property("arguments").get());
    list->push_back(make_value<double>(1.0));
    list->push_back(make_value<double>(3.0));
    CHECK(dynamic->value(zero_context()) == 4.0);
}

TEST_CASE("Test ApplyToList node", "[node]") {
    auto apply = make_node_with_name<Node<vector<double>>>("ApplyToList<Real>");
    auto add = make_node_with_name<Node<double>>("Add");
    add->get_property("a")->set_any(0.5);
    apply->set_property("source", add);
    apply->get_property("property_name")->set_any(string("b"));
    auto args = apply->get_property("dynamic_arguments");
    REQUIRE(args != nullptr);
    auto list = dynamic_cast<UntypedListValue*>(args.get());
    REQUIRE(list != nullptr);
    CHECK(apply->value(zero_context()) == vector<double>{});
    SECTION("Simple") {
        list->push_back(make_value<double>(1.0));
        CHECK(apply->value(zero_context()) == vector<double>{1.5});
        list->push_back(make_value<double>(3.0));
        CHECK((apply->value(zero_context()) == vector<double>{1.5, 3.5}));
    }
    SECTION("More complex") {
        // This isn't really required..
        auto time_map = make_node_with_name<Node<double>>("TimeMap<Real>");
        time_map->get_property("offset")->set_any(Time(1.0));
        time_map->set_property("source", make_node_with_name<AbstractValue>("Linear"));
        REQUIRE(time_map->value(zero_context()) == 1.0);
        list->push_back(time_map);
        CHECK(apply->value(zero_context()) == vector<double>{1.5});
    }
}

TEST_CASE("Test DynamicListZip node", "[node]") {
    auto zip = make_node_with_name<Node<vector<double>>>("DynamicListZip<Real>");
    zip->get_property("node_type")->set_any(string("Add"));
    auto args = zip->get_property("arguments_list");
    auto list_of_lists = dynamic_cast<UntypedListValue*>(args.get());
    auto a_list = make_shared<UntypedListValue>();
    auto b_list = make_shared<UntypedListValue>();
    list_of_lists->push_back(a_list);
    list_of_lists->push_back(b_list);
    CHECK(zip->value(zero_context()) == vector<double>{});
    a_list->push_back(make_value<double>(1.0));
    b_list->push_back(make_value<double>(1.5));
    CHECK(zip->value(zero_context()) == vector<double>{2.5});
    a_list->push_back(make_value<double>(2.0));
    b_list->push_back(make_value<double>(3.5));
    CHECK((zip->value(zero_context()) == vector<double>{2.5, 5.5}));
}
