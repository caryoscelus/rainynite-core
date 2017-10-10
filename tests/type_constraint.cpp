/*  type_constraint.cpp - test type constraint
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

#include <core/std/string.h>
#include <core/node/type_constraint.h>

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("Type constraint initialization", "[node]") {
    TypeConstraint int_only { typeid(int) };
    TypeConstraint numbers { { typeid(int), typeid(double) } };
    TypeConstraint anything;
    CHECK(int_only.accept(typeid(int)));
    CHECK(!int_only.accept(typeid(double)));
    CHECK(!int_only.accept(typeid(string)));
    CHECK(numbers.accept(typeid(int)));
    CHECK(numbers.accept(typeid(double)));
    CHECK(!numbers.accept(typeid(string)));
    CHECK(anything.accept(typeid(int)));
    CHECK(anything.accept(typeid(double)));
    CHECK(anything.accept(typeid(string)));
}

TEST_CASE("Type constraint synonymous types", "[node]") {
    TypeConstraint int_only { Only<int>() };
    CHECK(int_only == TypeConstraint { typeid(int) });
}
