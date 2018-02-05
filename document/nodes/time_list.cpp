/*  time_list.cpp - TimeList node
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

#include <core/node_info/macros.h>
#include <core/node_info/default_node.h>
#include <core/node/proxy_node.h>
#include <core/all_types.h>
#include <core/context.h>

namespace rainynite::core::nodes {

// TODO: use List<Time> instead of TimePeriod and Time parameters?
template <typename T>
class TimeList :
    public NewNode<
        TimeList<T>,
        vector<T>,
        types::Only<T>,
        types::Only<TimePeriod>,
        types::Only<Time>
    >
{
    DOC_STRING(
        "Node that makes a list of its source at different times."
    )

    NODE_PROPERTIES("source", "period", "step")
    COMPLEX_DEFAULT_VALUES(make_default_node<T>(), make_value<TimePeriod>(), make_value<Time>())
    PROPERTY(source)
    PROPERTY(period)
    PROPERTY(step)

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        auto source = p_source();
        auto period = period_value<TimePeriod>(ctx);
        auto step = step_value<Time>(ctx);
        vector<NodeInContext> result;
        for (auto t = period.get_first(); t < period.get_last(); t += step) {
            auto nctx = make_shared<Context>(*ctx);
            nctx->set_time(t);
            result.emplace_back(source, nctx);
        }
        return result;
    }
};

NODE_INFO_TEMPLATE(TimeList, TimeList<T>, vector<T>);
TYPE_INSTANCES(TimeListNodeInfo)

} // namespace rainynite::core::nodes
