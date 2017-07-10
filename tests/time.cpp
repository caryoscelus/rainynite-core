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

#include <core/time/time_period.h>

using namespace core;

TEST_CASE("Time sanity", "[time]") {
    Time a(3, 8, 4);
    Time b(a);
    CHECK(b == a);
    CHECK(a+b == Time(7, 8, 0));
    CHECK(Time(0, 12, 24) == Time(2, 12, 0));
}

TEST_CASE("Time FPS conversion", "[time]") {
    auto t = Time(10.0);
    t.set_fps(12);
    t.change_fps(24);
    REQUIRE(t.get_seconds() == 5.0);
}

TEST_CASE("Time arithmetics", "[time]") {
    CHECK(Time(1)+Time(3) == Time(4));
    CHECK(Time()+Time(1) == Time(1));
    CHECK(Time(4)-Time(5) < Time());

    CHECK(Time(4)*5 == Time(20));

    CHECK(++Time(4) == Time(5));
    CHECK(--Time(0, 20, 5) == Time(0, 20, 4));
}

TEST_CASE("Time FPS mismatch", "[time]") {
    CHECK(Time(1, 12, 10) > Time(1, 24, 5));
    auto t = Time(1, 12, 10) + Time(1, 24, 5);
    CHECK(t == Time(3, 24, 1));
}

void test_period(TimePeriod const& period, unsigned amount) {
    unsigned i = 0;
    for (auto t : period) {
        (void) t;
        ++i;
    }
    REQUIRE(i == amount);
}

TEST_CASE("Time period iterator", "[time]") {
    test_period(TimePeriod(Time(0), Time(4)), 4);
    test_period(TimePeriod(Time(0), Time(4.5)), 5);
    test_period(TimePeriod(Time(0, 8), Time(5, 8)), 40);
}
