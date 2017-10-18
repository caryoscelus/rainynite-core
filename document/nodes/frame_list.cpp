/*  frame_list.cpp - node that switches between its children in time
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

template <class T>
class FrameList : public ProxyNode<T> {
    DOC_STRING(
        "Value based on list of time-value mappings"
    )

public:
    FrameList() {
        this->template init_list<TimePoint<T>>(frame_list);
        this->template init<T>(default_value, {});

        // TODO: don't do this here?
        this->get_frame_list()->new_id();
    }

    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto flist = dynamic_pointer_cast<BaseValue<vector<TimePoint<T>>>>(list_frame_list());
        if (flist == nullptr)
            throw NodeAccessError("FrameList: invalid frame_list");
        auto frames = flist->get(ctx);
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
        if (last)
            return {last->value, ctx};
        return {get_default_value(), ctx};
    }
private:
    NODE_LIST_PROPERTY(frame_list, TimePoint<T>);
    NODE_PROPERTY(default_value, T);
};

NODE_INFO_TEMPLATE(FrameList, FrameList<T>, T);
TYPE_INSTANCES(FrameListNodeInfo)

} // namespace rainynite::core::nodes
