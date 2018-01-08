/*  nodes/test/benchmark.cpp - benchmark other nodes execution times
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

#include <chrono>

#include <core/node_info.h>
#include <core/node/new_node.h>
#include <core/context.h>
#include <core/document.h>
#include <core/exceptions.h>

namespace rainynite::core::nodes {

class BenchmarkTimePeriod :
    public NewNode<
        BenchmarkTimePeriod,
        Time,
        types::Any,
        types::Only<TimePeriod>
    >
{
    DOC_STRING(
        "Measure exec time of child node\n"
        "\n"
    )

    NODE_PROPERTIES("target", "time_period")
    DEFAULT_VALUES(Nothing{}, TimePeriod{})

    PROPERTY(target)
    PROPERTY(time_period)

protected:
    Time get(shared_ptr<Context> ctx) const override {
        using clock = std::chrono::high_resolution_clock;
        using duration = std::chrono::duration<double>;

        auto period = time_period_value<TimePeriod>(ctx);
        auto trgt = p_target();
        auto nctx = make_shared<Context>(*ctx);

        auto start_time = clock::now();

        for (auto time : period) {
            nctx->set_time(time);
            trgt->get_any(nctx);
        }

        duration r = clock::now() - start_time;

        return Time{r.count()};
    }
};

REGISTER_NODE(BenchmarkTimePeriod);

} // namespace rainynite::core::nodes
