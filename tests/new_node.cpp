/*  new_node.cpp - test new node system
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

#include <core/node/new_node.h>
#include <core/node/make.h>
#include "zero_context.h"

using namespace rainynite;
using namespace rainynite::core;

class Add :
    public NewNode<
        Add,
        double,
        types::Only<double>,
        types::Only<double>
    >
{
    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(0.0, 0.0)
    PROPERTY(a)
    PROPERTY(b)

    DOC_STRING(
        "Simple Add node for testing..."
    )

    double get(shared_ptr<Context> ctx) const override {
        return a_value<double>(ctx) + b_value<double>(ctx);
    }
};

TEST_CASE("New node system: simple Add node", "[node]") {
    auto add = make_shared<Add>();
    CHECK(add->value(zero_context()) == 0);
    add->set_property("a", make_value<double>(1));
    CHECK(add->value(zero_context()) == 1);
    add->set_property("b", make_value<double>(2));
    CHECK(add->value(zero_context()) == 3);
}
