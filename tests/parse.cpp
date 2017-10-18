/*  parse.cpp - parse testing
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

#include <core/exceptions.h>
#include <core/time/parse.h>

using namespace rainynite;
using namespace rainynite::core;

TEST_CASE("Parse time", "[time,parse]") {
    CHECK(parse_time("0f@12") == Time(0, 12, 0));
    CHECK(parse_time("12f@12") == Time(1, 12, 0));
    CHECK(parse_time("55f@12") == Time(4, 12, 7));
}

TEST_CASE("Parse time exceptions", "[time,parse]") {
    CHECK_THROWS_AS(parse_time("12:15"), ParseError);
    CHECK_THROWS_AS(parse_time("5f@"), ParseError);
    CHECK_THROWS_AS(parse_time("5.5@12"), ParseError);
}


TEST_CASE("Parse time period", "[time,parse]") {
    CHECK(parse_time_period("[0f@12, 2f@12)") == TimePeriod(Time(0, 12, 0), Time(0, 12, 2)));
}

TEST_CASE("Parse time period exceptions", "[time,parse]") {
    CHECK_THROWS_AS(parse_time_period("[0f@12, 2f@12"), ParseError);
}
