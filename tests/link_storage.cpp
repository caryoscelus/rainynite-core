/*  link_storage.cpp - test link storage
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

#include <core/node/link_storage.h>
#include <core/node/make.h>
#include "zero_context.h"

using namespace rainynite;
using namespace rainynite::core;

class RealOnly :
    public LinkStorage<
        types::Only<double>,
        types::Only<double>
    >
{
};

TEST_CASE("Link storage: only", "[node]") {
    auto real = make_unique<RealOnly>();
    auto zero = make_value<double>(0);
    auto one = make_value<double>(1);
    real->set_link(0, zero);
    real->set_link(1, one);
    CHECK(real->get_link_as<double>(0)->value(zero_context()) == 0);
    CHECK(real->get_link_as<double>(1)->value(zero_context()) == 1);
    CHECK_THROWS_AS((real->set_link(0, make_value<string>())), NodeAccessError);
}

class RealOrString :
    public LinkStorage<
        types::AnyOf<double, string>
    >
{
};

TEST_CASE("Link storage: any of", "[node]") {
    auto links = make_unique<RealOrString>();
    auto zero = make_value<double>(0);
    auto hello = make_value<string>("hello");
    links->set_link(0, zero);
    CHECK(links->get_link_as<double>(0)->value(zero_context()) == 0);
    links->set_link(0, hello);
    CHECK(links->get_link_as<string>(0)->value(zero_context()) == "hello");
    CHECK_THROWS_AS((links->set_link(0, make_value<int>())), NodeAccessError);
}

class AnyLink :
    public LinkStorage<
        types::Any
    >
{
};

TEST_CASE("Link storage: any type", "[node]") {
    auto links = make_unique<RealOrString>();
    auto zero = make_value<double>(0);
    auto hello = make_value<string>("hello");
    links->set_link(0, zero);
    CHECK(links->get_link_as<double>(0)->value(zero_context()) == 0);
    links->set_link(0, hello);
    CHECK(links->get_link_as<string>(0)->value(zero_context()) == "hello");
}
