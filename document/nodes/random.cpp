/*  random.cpp - random-generating nodes
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

#include <random>

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/context.h>

namespace rainynite::core::nodes {

// TODO: use generate_canonical instead
template <typename R>
double get_random_in_range(R& engine, double max) {
    static_assert(R::min() == 0);
    double random = engine() * max;
    random /= R::max()+1.0;
    return random;
}

class RandomList :
    public NewNode<
        RandomList,
        vector<double>,
        types::Only<double>,
        types::Only<double>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Node that generates list of static random numbers.\n"
        "\n"
        "Currently it returns doubles in [0; 1) range because that's more"
        "useful than returning random 32bit ints and strict precision isn't"
        "that important yet."
    )

    NODE_PROPERTIES("length", "seed", "max")
    DEFAULT_VALUES(0.0, 0.0, 1.0)
    PROPERTY(length)
    PROPERTY(seed)
    PROPERTY(max)

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        vector<NodeInContext> result;
        random_sequence(ctx, [&result, ctx](auto r) {
            result.emplace_back(make_value<double>(r), ctx);
        });
        return result;
    }

public:
    size_t list_links_count(shared_ptr<Context> ctx) const override {
        return std::max(length_value<double>(ctx), 0.0);
    }

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
        int length = length_value<double>(ctx);
        if (length < 1)
            return;
        auto seed = seed_value<double>(ctx);
        auto max = max_value<double>(ctx);
        random_engine.seed(seed);
        // TODO: cache
        while (length--) {
            auto random = get_random_in_range(random_engine, max);
            f(random);
        }
    }
private:
    mutable std::mt19937 random_engine;
};

REGISTER_NODE(RandomList);


class MovingRandom :
    public NewNode<
        MovingRandom,
        double,
        types::Only<double>,
        types::Only<Time>,
        types::Only<double>
    >
{
    DOC_STRING(
        ""
    )

    NODE_PROPERTIES("max", "period", "seed")
    DEFAULT_VALUES(1.0, Time{1}, 0.0)
    PROPERTY(max)
    PROPERTY(period)
    PROPERTY(seed)

protected:
    double get(shared_ptr<Context> ctx) const override {
        auto max = max_value<double>(ctx);
        auto period = period_value<Time>(ctx);
        auto seed = seed_value<double>(ctx);

        random_engine.seed(seed);
        double p = ctx->get_time().get_seconds() / period.get_seconds();
        random_engine.discard((int)std::floor(p));
        auto a = get_random_in_range(random_engine, max);
        auto b = get_random_in_range(random_engine, max);
        auto offset = p - std::floor(p);
        return a*(1-offset)+b*offset;
    }
private:
    mutable std::mt19937 random_engine;
};

REGISTER_NODE(MovingRandom);

} // namespace rainynite::core::nodes
