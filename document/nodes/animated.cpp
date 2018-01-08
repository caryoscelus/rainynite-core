/*  animated.cpp - node that switches between its children in time
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

#include <geom_helpers/knots.h>

#include <core/node_info/macros.h>
#include <core/node/proxy_node.h>
#include <core/util/type_info.h>
#include <core/all_types.h>
#include <core/context.h>

namespace rainynite::core::nodes {

template <class T>
class Animated : public ProxyNode<T> {
public:
    Animated() {
        this->template init_list<T>(children);
        this->template init_list<TimePeriod>(periods);
        this->init(default_value, T());
    }

    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        size_t i = 0;
        auto time = ctx->get_time();
        for (auto period : get_periods()->value(ctx)) {
            period.set_fps(time.get_fps());
            if (period.contains(time)) {
                auto nctx = make_shared<Context>(*ctx);
                nctx->set_time(calculate_time(period, time));
                return { list_children()->get_link(i), nctx };
            }
            ++i;
        }
        return { get_default_value(), ctx };
    }

    void add_child(TimePeriod period, AbstractReference ref) {
        push_value(list_periods(), period);
        list_children()->push_back(dynamic_pointer_cast<BaseValue<T>>(ref));
    }

    size_t child_count() const {
        return list_children()->link_count();
    }

private:
    Time calculate_time(TimePeriod const& period, Time time) const {
        auto a = period.get_first();
        auto b = period.get_last();
        return (time-a)/(b-a).get_seconds();
    }

    NODE_LIST_PROPERTY(children, T);
    NODE_LIST_PROPERTY(periods, TimePeriod);
    NODE_PROPERTY(default_value, T);
};

NODE_INFO_TEMPLATE(Animated, Animated<T>, T);
TYPE_INSTANCES(AnimatedNodeInfo)

} // namespace rainynite::core::nodes
