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

#include <core/node.h>

using namespace core;

class Add : public Node<Real> {
public:
    Add() {
        init<Real>(a, 0);
        init<Real>(b, 0);
    }
public:
    virtual Real get(Time t) const override {
        auto a = get_a()->get(t);
        auto b = get_b()->get(t);
        return a+b;
    }

    NODE_PROPERTY(a, Real);
    NODE_PROPERTY(b, Real);
};

TEST_CASE("Test Add node", "[node]") {
    auto add = std::make_shared<Add>();
    auto one = std::make_shared<Value<Real>>(1.0);
    auto two = std::make_shared<Value<Real>>(2.0);
    CHECK(add->get(Time()) == 0.0);
    add->set_a(one);
    CHECK(add->get(Time()) == 1.0);
    add->set_property("b", two);
    CHECK(add->get(Time()) == 3.0);
}
