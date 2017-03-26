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

using Real = double;
template <typename T>
using List = std::vector<T>;

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
    one->set(4.0);
    CHECK(add->get(Time()) == 6.0);
}

class Sum : public Node<Real> {
public:
    Sum() {
        init<List<Real>>(list, {});
    }
public:
    virtual Real get(Time t) const override {
        auto list = get_list()->get(t);
        Real result = 0;
        for (auto const& x : list)
            result += x;
        return result;
    }

    NODE_PROPERTY(list, List<Real>);
};

TEST_CASE("Sum Node", "[node]") {
    auto list = std::make_shared<Value<List<Real>>>();
    auto sum = std::make_shared<Sum>();
    CHECK(sum->get(Time()) == 0.0);
    sum->set_property("list", list);
    CHECK(sum->get(Time()) == 0.0);
    list->mod().push_back(1.0);
    CHECK(sum->get(Time()) == 1.0);
    list->mod().push_back(4.0);
    CHECK(sum->get(Time()) == 5.0);
}

class SumNode : public Node<Real> {
public:
    SumNode() {
        init<List<BaseReference<Real>>>(list, {});
    }
public:
    virtual Real get(Time t) const override {
        auto list = get_list()->get(t);
        Real result = 0;
        for (auto x : list)
            result += x->get(t);
        return result;
    }

    NODE_PROPERTY(list, List<BaseReference<Real>>);
};

TEST_CASE("Real node sum", "[node]") {
    auto list = std::make_shared<Value<List<BaseReference<Real>>>>();
    auto sum = std::make_shared<SumNode>();
    CHECK(sum->get(Time()) == 0.0);
    sum->set_property("list", list);
    CHECK(sum->get(Time()) == 0.0);
    auto one = std::make_shared<Value<Real>>(1.0);
    list->mod().push_back(one);
    CHECK(sum->get(Time()) == 1.0);
    list->mod().push_back(one);
    CHECK(sum->get(Time()) == 2.0);
    one->set(3.0);
    CHECK(sum->get(Time()) == 6.0);
}
