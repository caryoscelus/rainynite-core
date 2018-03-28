/*  frame_list.cpp - node that switches between its children in time
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

template <class T>
class FrameList :
    public NewProxyNode<
        FrameList<T>,
        T,
        types::Only<T>,
        types::Only<vector<TimePoint<T>>>,
        types::Only<bool>
    >
{
    DOC_STRING(
        "Value based on list of time-value mappings"
    )

    NODE_PROPERTIES("default_value", "frame_list", "reset_time")
    COMPLEX_DEFAULT_VALUES(make_default_node<T>(), make_default_node<vector<TimePoint<T>>>(), make_value<bool>(false))
    PROPERTY(default_value)
    PROPERTY(frame_list)
    PROPERTY(reset_time)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto frames = frame_list_value<vector<TimePoint<T>>>(ctx);
        std::sort(
            std::begin(frames),
            std::end(frames),
            [](auto a, auto b) -> bool {
                return a.time < b.time;
            }
        );
        auto time = ctx->get_time();
        TimePoint<T> const* last = nullptr;
        for (auto const& frame : frames) {
            if (!frame.value)
                continue;
            if (frame.time == time)
                return {frame.value, ctx};
            if (frame.time > time)
                break;
            last = &frame;
        }
        if (last) {
            if (reset_time_value<bool>(ctx)) {
                ctx = make_shared<Context>(*ctx);
                ctx->set_time(time-last->time);
            }
            return {last->value, ctx};
        }
        return {p_default_value(), ctx};
    }
};

NODE_INFO_TEMPLATE(FrameList, FrameList<T>, T);
TYPE_INSTANCES(FrameListNodeInfo)

} // namespace rainynite::core::nodes
