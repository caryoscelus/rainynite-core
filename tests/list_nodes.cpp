/*  list_nodes.cpp - test list-related nodes
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

TEST_CASE("Test ToTypedList node", "[node]") {
    auto ttlist = make_node_with_name<Node<vector<double>>>("ToTypedList<Real>");
    auto untyped = dynamic_cast<UntypedListValue*>(ttlist->get_property("source").get());
    CHECK(ttlist->value(zero_context()) == vector<double>{});
    untyped->push_back(make_value<double>(0.5));
    CHECK(ttlist->value(zero_context()) == vector<double>{0.5});
    untyped->push_back(make_value<double>(1.0));
    CHECK((ttlist->value(zero_context()) == vector<double>{0.5, 1.0}));
}
