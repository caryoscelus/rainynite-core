/*  interpolate.cpp - key-framed interpolate node
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
class Interpolate : public ProxyNode<T> {
    DOC_STRING(
        "Key-frames + interpolation between them.\n"
        "\n"
        "Interpolation node should have properties a and b of type T"
        "and property progress of type double that should accept values"
        "in [0; 1] range."
    )

public:
    Interpolate() {
        this->template init<string>(interpolate_with, {});
        this->template init_list<TimePoint<T>>(keyframes);
        this->template init<double>(smoothing, {});
        this->template init<T>(default_value, {});
    }
public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto frames = get_keyframes()->get(ctx);
        if (frames.empty()) {
            // cannae do nothing if you ain't got no frames!
            return {get_default_value(), ctx};
        }

        auto time = ctx->get_time();
        observer_ptr<TimePoint<T> const> left;
        observer_ptr<TimePoint<T> const> right;
        for (auto const& frame : frames) {
            if (frame.time == time) {
                // exact hit: no interpolation needed
                return {frame.value, ctx};
            }
            if ((!left || left->time < frame.time) && frame.time < time) {
                left.reset(&frame);
            } else if ((!right || right->time > frame.time) && frame.time > time) {
                right.reset(&frame);
            }
        }
        if (!left && !right)
            throw std::logic_error("Non-empty frame list should contain at least one element");
        if (!left || !right) {
            return {(left ? left : right)->value, ctx};
        }

        auto nctx = make_shared<Context>(*ctx);
        nctx->set_time(left->time);
        auto left_smoothing = get_smoothing()->get(nctx);
        nctx->set_time(right->time);
        auto right_smoothing = get_smoothing()->get(nctx);
        auto here_smoothing = get_smoothing()->get(ctx);

        if (left_smoothing == right_smoothing)
            throw std::invalid_argument("Cannot calculate progress when smoothings are equal");
        auto progress = make_value<double>((here_smoothing - left_smoothing)/(right_smoothing - left_smoothing));

        // TODO: cache interpolate node
        auto interpolate = make_node_with_name<Node<T>>(get_interpolate_with()->get(ctx));
        interpolate->set_property("a", left->value);
        interpolate->set_property("b", right->value);
        interpolate->set_property("progress", progress);
        return {interpolate, ctx};
    }
private:
    NODE_PROPERTY(interpolate_with, string);
    NODE_LIST_PROPERTY(keyframes, TimePoint<T>);
    NODE_PROPERTY(smoothing, double);
    NODE_PROPERTY(default_value, T);
};

NODE_INFO_TEMPLATE(Interpolate, Interpolate<T>, T);
TYPE_INSTANCES(InterpolateNodeInfo)

} // namespace rainynite::core::nodes
