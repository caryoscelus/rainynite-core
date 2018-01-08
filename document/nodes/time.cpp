/*  time.cpp - time-related nodes
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

#include <core/all_types.h>
#include <core/node_info/macros.h>
#include <core/node/proxy_node.h>
#include <core/context.h>

namespace rainynite::core::nodes {

template <typename T>
class TimeMap :
    public NewProxyNode<
        TimeMap<T>,
        T,
        types::Only<T>,
        types::Only<double>,
        types::Only<Time>
    >
{
    DOC_STRING(
        "Time map"
    )

    NODE_PROPERTIES("source", "multiplier", "offset")
    DEFAULT_VALUES(T{}, 1.0, Time{})
    PROPERTY(source)
    PROPERTY(multiplier)
    PROPERTY(offset)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto t = ctx->get_time() * multiplier_value<double>(ctx) + offset_value<Time>(ctx);
        auto nctx = make_shared<Context>(*ctx);
        nctx->set_time(t);
        return { p_source(), nctx };
    }
};

NODE_INFO_TEMPLATE(TimeMap, TimeMap<T>, T);
TYPE_INSTANCES(TimeMapNodeInfo)


template <typename T>
class AtTime :
    public NewProxyNode<
        AtTime<T>,
        T,
        types::Only<T>,
        types::Only<Time>
    >
{
    DOC_STRING(
        "Get value of its child at given time."
    )

    NODE_PROPERTIES("source", "time")
    DEFAULT_VALUES(T{}, Time{});
    PROPERTY(source)
    PROPERTY(time)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto nctx = make_shared<Context>(*ctx);
        nctx->set_time(time_value<Time>(ctx));
        return { p_source(), nctx };
    }
};

NODE_INFO_TEMPLATE(AtTime, AtTime<T>, T);
TYPE_INSTANCES(AtTimeNodeInfo)


class Now :
    public NewNode<
        Now,
        Time
    >
{
    DOC_STRING(
        "Return current time"
    )
    NODE_PROPERTIES()
    DEFAULT_VALUES()

protected:
    Time get(shared_ptr<Context> ctx) const override {
        return ctx->get_time();
    }
};

REGISTER_NODE(Now);

template <typename T>
class TimeLoop :
    public NewProxyNode<
        TimeLoop<T>,
        T,
        types::Only<T>,
        types::Only<Time>,
        types::Only<Time>
    >
{
    DOC_STRING(
        "Time loop"
    )

    NODE_PROPERTIES("source", "period", "offset")
    DEFAULT_VALUES(T{}, Time{}, Time{});
    PROPERTY(source)
    PROPERTY(period)
    PROPERTY(offset)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto period = period_value<Time>(ctx);
        auto t = ctx->get_time() + offset_value<Time>(ctx);
        Time time;
        if (period <= Time()) {
            time = t;
        } else {
            time = t - std::floor(t/period)*period;
        }
        auto nctx = make_shared<Context>(*ctx);
        nctx->set_time(time);
        return { p_source(), nctx };
    }
};

NODE_INFO_TEMPLATE(TimeLoop, TimeLoop<T>, T);
TYPE_INSTANCES(TimeLoopNodeInfo)

} // namespace rainynite::core::nodes
