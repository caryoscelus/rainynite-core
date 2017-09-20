/*
 *  time_list.cpp - TimeList node
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
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/node/proxy_node.h>
#include <core/time/time_period.h>
#include <core/all_types.h>
#include <core/context.h>

namespace rainynite::core {
namespace nodes {

template <typename T>
class TimeList : public ProxyListNode<T> {
public:
    TimeList() {
        this->template init<T>(source, {});
        this->template init<TimePeriod>(period, {});
        this->template init<Time>(step, {});
    }
public:
    void step_into_list(shared_ptr<Context> ctx, std::function<void(NodeInContext)> f) const override {
        try {
            auto source = get_source();
            auto period = get_period()->get(ctx);
            auto step = get_step()->get(ctx);
            for (auto t = period.get_first(); t < period.get_last(); t += step) {
                auto nctx = make_shared<Context>(*ctx);
                nctx->set_time(t);
                f({source, nctx});
            }
        } catch (...) {
        }
    }

private:
    NODE_PROPERTY(source, T);
    NODE_PROPERTY(period, TimePeriod);
    NODE_PROPERTY(step, Time);
};

NODE_INFO_TEMPLATE(TimeList, TimeList<T>, vector<T>);
TYPE_INSTANCES(TimeListNodeInfo)

} // namespace nodes
} // namespace rainynite::core
