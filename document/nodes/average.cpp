/*  average.cpp - universal average node
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

#include <core/node_info.h>
#include <core/node/proxy_node.h>
#include <core/all_types.h>
#include <core/context.h>

namespace rainynite::core::nodes {

namespace detail {

using std::declval;

template <typename A, typename B>
using can_be_summed_t = decltype(declval<A&>() + declval<B&>());

template <typename A, typename B>
using can_be_multiplied_t = decltype(declval<A&>() * declval<B&>());

}

using std::experimental::is_detected_v;

template <typename A, typename B>
constexpr bool can_be_summed = is_detected_v<detail::can_be_summed_t, A, B>;

template <typename A, typename B>
constexpr bool can_be_multiplied = is_detected_v<detail::can_be_multiplied_t, A, B>;


template <class T>
class WeightedAverage : public Node<T> {
    DOC_STRING(
        "Weighted average value\n"
        "\n"
        "Works with any type that can be summed with itself"
        "and multiplied by Real."
    )

public:
    WeightedAverage() {
        this->template init<T>(a, {});
        this->template init<T>(b, {});
        this->template init<double>(progress, {});
    }

    T get(shared_ptr<Context> ctx) const override {
        if constexpr (can_be_summed<T, T> && can_be_multiplied<T, double>) {
            auto p = get_progress()->get(ctx);
            return get_a()->get(ctx)*(1.0-p) + get_b()->get(ctx)*p;
        } else {
            throw std::logic_error("WeightedAverage on unsupported type");
        }
    }

private:
    NODE_PROPERTY(a, T);
    NODE_PROPERTY(b, T);
    NODE_PROPERTY(progress, double);
};

NODE_INFO_TEMPLATE(WeightedAverage, WeightedAverage<T>, T);
TYPE_INSTANCES(WeightedAverageNodeInfo)

} // namespace rainynite::core::nodes
