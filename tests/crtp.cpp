/*  crtp.cpp - test curiously recurring template pattern helpers
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

#include <core/util/crtp.h>

using namespace crtp;

template <typename Self>
class Base : Crtp<Base<Self>, Self> {
public:
    void call_child() {
        this->self()->do_something();
    }
};

class Child : Base<Child> {
public:
    void call_parent() {
        do_something();
    }
    void do_something() {
        done = true;
    }
    bool has_done_something() const {
        return done;
    }
private:
    bool done = false;
};

TEST_CASE("Test Crtp", "[util]") {
    Child child;
    child.call_parent();
    CHECK(child.has_done_something());
}
