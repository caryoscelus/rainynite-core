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

AbstractReference make_time_list_0_1() {
    auto time_list = make_node_with_name<Node<vector<double>>>("TimeList/Real");
    time_list->get_property("step")->set_any(Time(1.0));
    time_list->get_property("period")->set_any(TimePeriod(Time(0.0), Time(2.0)));
    time_list->set_property("source", make_node_with_name<AbstractValue>("Linear"));
    auto untyped = make_node_with_name<Node<Nothing>>("ToUntypedList");
    untyped->set_property("source", time_list);
    return untyped;
}

TEST_CASE("Test ApplyToList node", "[node]") {
    auto apply = make_node_with_name<Node<vector<double>>>("ApplyToList/Real");
    auto add = make_node_with_name<AbstractNode>("Add/Real");
    add->get_property("a")->set_any(0.5);
    apply->set_property("source", dynamic_pointer_cast<AbstractValue>(std::move(add)));
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
        auto time_map = make_node_with_name<Node<double>>("TimeMap/Real");
        time_map->get_property("offset")->set_any(Time(1.0));
        time_map->set_property("source", make_node_with_name<AbstractValue>("Linear"));
        REQUIRE(time_map->value(zero_context()) == 1.0);
        list->push_back(time_map);
        CHECK(apply->value(zero_context()) == vector<double>{1.5});
    }
    SECTION("Preserve context") {
        auto time_list = make_time_list_0_1();
        apply->set_property("dynamic_arguments", time_list);
        auto exp = vector<double>{0.5, 1.5};
        CHECK(apply->value(zero_context()) == exp);
    }
}

TEST_CASE("Test DynamicListZip node", "[node]") {
    auto zip = make_node_with_name<Node<vector<double>>>("DynamicListZip/Real");
    zip->get_property("node_type")->set_any(string("Add/Real"));
    auto args = zip->get_property("arguments_list");
    auto list_of_lists = dynamic_cast<UntypedListValue*>(args.get());
    SECTION("Simple") {
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
    SECTION("Preserve context") {
        auto t_list = make_time_list_0_1();
        list_of_lists->push_back(t_list);
        list_of_lists->push_back(t_list);
        auto exp = vector<double>{0, 2};
        CHECK(zip->value(zero_context()) == exp);
    }
}
