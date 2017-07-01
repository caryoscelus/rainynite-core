/*
 *  notify.cpp - AbstractNotify tests
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

#include <core/node/notify.h>

using namespace core;

TEST_CASE("Notify", "") {
    AbstractNotify object;
    bool changed = false;
    unsigned count = 0;
    object.changed();
    CHECK(!changed);
    CHECK(count == 0);
    object.subscribe([&changed](){
        changed = true;
    });
    object.changed();
    CHECK(changed);
    CHECK(count == 0);
    object.subscribe([&count](){
        ++count;
    });
    object.changed();
    CHECK(count == 1);
    object.changed();
    CHECK(count == 2);
}
