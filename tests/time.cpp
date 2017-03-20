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

#include <core/time.h>

using namespace core;

void test_period(TimePeriod const& period, unsigned amount) {
    unsigned i = 0;
    for (auto t : period) {
        ++i;
    }
    REQUIRE(i == amount);
}

TEST_CASE("Time period iterator", "[time]") {
    test_period(TimePeriod(Time(0), Time(4)), 4);
    test_period(TimePeriod(Time(0), Time(4.5)), 5);
    test_period(TimePeriod(Time(0), Time(5), Time(1.0/8)), 40);
}
