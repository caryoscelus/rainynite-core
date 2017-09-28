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
