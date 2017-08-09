/*
 *  random.cpp - random-generating nodes
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

#include <random>

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>

#include <iostream>

namespace core {
namespace nodes {

/**
 * Node that generates list of static random numbers.
 *
 * Currently it returns doubles in [0; 1) range because that's more
 * useful than returning random 32bit ints and strict precision isn't
 * that important yet.
 */
class RandomList : public Node<std::vector<double>> {
public:
    RandomList() {
        init<double>(length, 0);
        init<double>(seed, 0);
    }
public:
    std::vector<double> get(Time time) const override {
        try {
            int length = get_length()->get(time);
            if (length < 1)
                return {};
            auto seed = get_seed()->get(time);
            random_engine.seed(seed);
            // TODO: cache
            std::vector<double> result;
            while (length--) {
                double random = random_engine();
                static_assert(decltype(random_engine)::min() == 0);
                random /= decltype(random_engine)::max()+1;
                result.push_back(random);
            }
            return result;
        } catch (...) {
            return {};
        }
    }
private:
    mutable std::mt19937 random_engine;
private:
    NODE_PROPERTY(length, double);
    NODE_PROPERTY(seed, double);
};

REGISTER_NODE(RandomList);

} // namespace nodes
} // namespace core
