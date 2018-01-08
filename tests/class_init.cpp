/*  clsas_init.cpp - class init tests
 *  Copyright (C) 2017-2018 caryoscelus
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

#include <core/util/class_init.h>

using namespace class_init;

bool& foo_registered() {
    static bool instance = false;
    return instance;
}

class Foo : Initialized<Foo> {
public:
    static void init() {
        foo_registered() = true;
    }
};

TEST_CASE("Automatic class initialization", "") {
    CHECK(foo_registered());
}

class Bar {
};

class ClassName {
public:
    virtual string operator()() const = 0;
};

class BarName : public ClassName, Registered<BarName, Bar, ClassName>, ReverseRegistered<BarName, Bar, string> {
public:
    string operator()() const override {
        return "Bar";
    }
};

TEST_CASE("Automatic class registration", "") {
    CHECK((type_info<ClassName, string>(typeid(Bar)) == "Bar"));
    CHECK_THROWS_AS((type_info<ClassName, string>(typeid(Foo))), UnknownTypeError);
}

TEST_CASE("Reverse class registration", "") {
    CHECK(find_type<string>("Bar") == typeid(Bar));
    CHECK_THROWS_AS(find_type<string>("Foo"), TypeLookupError);
}

class FooBar {
public:
    virtual string name() const {
        return "FooBar";
    }
    virtual ~FooBar() {}
};

class FooBaz : public FooBar {
public:
    string name() const override {
        return "FooBaz";
    }
};

class PolyClassName {
public:
    virtual string operator()(any const& object) const = 0;
};

class FooBarName : public PolyClassName, Registered<FooBarName, FooBar*, PolyClassName> {
public:
    string operator()(any const& object) const override {
        auto p = any_cast<FooBar*>(object);
        return p->name();
    }
};

TEST_CASE("Polymorphic class name", "") {
    FooBar* obj = new FooBar();
    CHECK((any_info<PolyClassName, string>(obj) == "FooBar"));
    delete obj;
    obj = new FooBaz();
    CHECK((any_info<PolyClassName, string>(obj) == "FooBaz"));
    CHECK_THROWS_AS((any_info<PolyClassName, string>(new FooBaz()) == "FooBaz"), UnknownTypeError);
}
