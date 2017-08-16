/*
 *  time.cpp - time-related nodes
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

#include <core/all_types.h>
#include <core/node_info.h>
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/context.h>

namespace core {
namespace nodes {

template <typename T>
class TimeMap : public ProxyNode<T> {
public:
    TimeMap() {
        this->template init<T>(source, {});
        this->template init<double>(multiplier, 1);
        this->template init<Time>(offset, {});
    }
public:
    void step_into(std::shared_ptr<Context> ctx, std::function<void(NodeInContext)> f) const override {
        auto t = ctx->get_time() * get_multiplier()->get(ctx) + get_offset()->get(ctx);
        auto nctx = std::make_shared<Context>(*ctx);
        nctx->set_time(t);
        f({get_source(), nctx});
    }

private:
    NODE_PROPERTY(source, T);
    NODE_PROPERTY(multiplier, double);
    NODE_PROPERTY(offset, Time);
};

NODE_INFO_TEMPLATE(TimeMap, TimeMap<T>, T);
TYPE_INSTANCES(TimeMapNodeInfo)

class Now : public Node<Time> {
public:
    Now() {
    }
public:
    Time get(std::shared_ptr<Context> ctx) const override {
        return ctx->get_time();
    }
};

REGISTER_NODE(Now);

} // namespace nodes
} // namespace core
