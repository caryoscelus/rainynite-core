/*  nullptr.cpp - nullptr helpers tests
 *  Copyright (C) 2018 caryoscelus
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

#include <core/std/memory.h>
#include <core/nullptr.h>

using namespace rainynite;

TEST_CASE("Test nullptr throw", "") {
    CHECK_THROWS_AS(no_null(nullptr), NullPointerException);
    shared_ptr<int> x;
    CHECK_THROWS_AS(no_null(x), NullPointerException);
}
