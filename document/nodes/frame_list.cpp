/*
 *  frame_list.cpp - node that switches between its children in time
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

namespace rainynite::core {
namespace nodes {

template <typename T>
class Frame : public Node<TimePointType> {
public:
    Frame() {
        this->template init<T>(value, {});
        this->template init<Time>(time, {});
    }
public:
    TimePointType get(std::shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
private:
    NODE_PROPERTY(value, T);
    NODE_PROPERTY(time, Time);
};

NODE_INFO_TEMPLATE(Frame, Frame<T>, TimePointType);
TYPE_INSTANCES(FrameNodeInfo)

template <class T>
class FrameList : public ProxyNode<T> {
public:
    FrameList() {
        this->template init_list<TimePointType>(frame_list);
        this->template init<T>(default_value, {});

        // TODO: don't do this here?
        this->get_frame_list()->new_id();
    }
public:
    NodeInContext get_proxy(std::shared_ptr<Context> ctx) const override {
        auto aframes = list_frame_list()->get_links();
        std::vector<TimePoint<T>> frames;
        std::transform(
            std::begin(aframes),
            std::end(aframes),
            std::back_inserter(frames),
            [ctx](auto frame) -> TimePoint<T> {
                if (auto f = std::dynamic_pointer_cast<Frame<T>>(frame))
                    return { f->get_time()->get(ctx), f->get_value() };
                return { Time::infinity(), nullptr };
            }
        );
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
    NODE_LIST_PROPERTY(frame_list, TimePointType);
    NODE_PROPERTY(default_value, T);
};

NODE_INFO_TEMPLATE(FrameList, FrameList<T>, T);
TYPE_INSTANCES(FrameListNodeInfo)

} // namespace nodes
} // namespace rainynite::core
