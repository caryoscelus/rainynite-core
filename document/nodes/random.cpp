/*  random.cpp - random-generating nodes
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
#include <core/context.h>

namespace rainynite::core {
namespace nodes {

// TODO: use generate_canonical instead
template <typename R>
double get_random_in_range(R& engine, double max) {
    static_assert(R::min() == 0);
    double random = engine() * max;
    random /= R::max()+1.0;
    return random;
}

/**
 * Node that generates list of static random numbers.
 *
 * Currently it returns doubles in [0; 1) range because that's more
 * useful than returning random 32bit ints and strict precision isn't
 * that important yet.
 */
class RandomList : public Node<vector<double>> {
public:
    RandomList() {
        init<double>(length, 0);
        init<double>(seed, 0);
        init<double>(max, 1);
    }
protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        vector<NodeInContext> result;
        random_sequence(ctx, [&result, ctx](auto r) {
            result.emplace_back(make_value<double>(r), ctx);
        });
        return result;
    }
public:
    vector<double> get(shared_ptr<Context> ctx) const override {
        vector<double> result;
        random_sequence(ctx, [&result](auto r) {
            result.push_back(r);
        });
        return result;
    }
private:
    template <typename F>
    void random_sequence(shared_ptr<Context> ctx, F f) const {
        try {
            int length = get_length()->get(ctx);
            if (length < 1)
                return;
            auto seed = get_seed()->get(ctx);
            auto max = get_max()->get(ctx);
            random_engine.seed(seed);
            // TODO: cache
            while (length--) {
                auto random = get_random_in_range(random_engine, max);
                f(random);
            }
        } catch (...) {
        }
    }
private:
    mutable std::mt19937 random_engine;
private:
    NODE_PROPERTY(length, double);
    NODE_PROPERTY(seed, double);
    NODE_PROPERTY(max, double);
};

REGISTER_NODE(RandomList);

class MovingRandom : public Node<double> {
public:
    MovingRandom() {
        init<double>(max, 1);
        init<double>(period, 1);
        init<double>(seed, 0);
    }
public:
    double get(shared_ptr<Context> ctx) const {
        try {
            auto max = get_max()->get(ctx);
            auto period = get_period()->get(ctx);
            auto seed = get_seed()->get(ctx);

            random_engine.seed(seed);
            double p = ctx->get_time().get_seconds() / period;
            random_engine.discard((int)std::floor(p));
            auto a = get_random_in_range(random_engine, max);
            auto b = get_random_in_range(random_engine, max);
            auto offset = p - std::floor(p);
            return a*(1-offset)+b*offset;
        } catch (...) {
            return 0;
        }
    }
private:
    mutable std::mt19937 random_engine;
private:
    NODE_PROPERTY(max, double);
    NODE_PROPERTY(period, double);
    NODE_PROPERTY(seed, double);
};

REGISTER_NODE(MovingRandom);

} // namespace nodes
} // namespace rainynite::core
