/*  time_nodes.cpp - test time-related nodes
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

#include "zero_context.h"

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("Test TimeList node", "[node]") {
    auto time_list = make_node_with_name<Node<vector<double>>>("TimeList/Real");
    time_list->get_property("step")->set_any(Time(1.0));
    time_list->get_property("period")->set_any(TimePeriod(Time(0.0), Time(2.0)));
    time_list->get_property("source")->set_any(0.0);
    CHECK((time_list->value(zero_context()) == vector<double>{0, 0}));
    time_list->set_property("source", make_node_with_name<AbstractValue>("Linear"));
    CHECK((time_list->value(zero_context()) == vector<double>{0, 1}));
}
