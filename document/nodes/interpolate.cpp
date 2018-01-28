/*  interpolate.cpp - key-framed interpolate node
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
#include <core/node/cast.h>
#include <core/all_types.h>
#include <core/context.h>
#include <core/util/nullptr.h>

namespace rainynite::core::nodes {

template <class T>
class Interpolate :
    public NewProxyNode<
        Interpolate<T>,
        T,
        types::Only<T>,
        types::Only<string>,
        types::Only<vector<TimePoint<T>>>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Key-frames + interpolation between them.\n"
        "\n"
        "Interpolation node should have properties a and b of type T\n"
        "and property progress of type double that should accept values\n"
        "in [0; 1] range."
    )

    NODE_PROPERTIES("default_value", "interpolate_with", "keyframes", "smoothing")
    COMPLEX_DEFAULT_VALUES(make_default_node<T>(), make_value<string>(), make_default_node<vector<TimePoint<T>>>(), make_node_with_name("Linear"))

    PROPERTY(default_value)
    PROPERTY(interpolate_with)
    PROPERTY(keyframes)
    PROPERTY(smoothing)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto frames = keyframes_value<vector<TimePoint<T>>>(ctx);
        if (frames.empty()) {
            // cannae do nothing if you ain't got no frames!
            return {p_default_value(), ctx};
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
        auto left_smoothing = smoothing_value<double>(nctx);
        nctx->set_time(right->time);
        auto right_smoothing = smoothing_value<double>(nctx);
        auto here_smoothing = smoothing_value<double>(ctx);

        if (left_smoothing == right_smoothing)
            throw std::invalid_argument("Cannot calculate progress when smoothings are equal");
        auto progress = make_value<double>((here_smoothing - left_smoothing)/(right_smoothing - left_smoothing));

        // TODO: cache interpolate node
        auto node_name = interpolate_with_value<string>(ctx);
        auto interpolate = make_node_with_name_as<AbstractNode>(node_name);
        auto interpolate_value = abstract_value_cast(interpolate);
        if (interpolate == nullptr)
            throw std::invalid_argument("Cannot make node named "+node_name);
        if (interpolate_value->get_type() != typeid(T))
            throw std::invalid_argument("Node type mismatch");
        interpolate->set_property("a", left->value);
        interpolate->set_property("b", right->value);
        interpolate->set_property("progress", progress);
        return {interpolate_value, ctx};
    }

public:
    bool can_set_any_at() const override {
        auto l = list_cast(p_keyframes());
        return l && l->is_editable_list();
    }

    void set_any_at(any const& value, shared_ptr<Context> ctx) override {
        auto time = ctx->get_time();
        auto l = no_null(list_cast(p_keyframes()));
        l->push_new();
        auto frame = l->get_link(l->link_count()-1);
        auto frame_node = no_null(abstract_node_cast(frame));
        frame_node->get_property("time")->set_any(time);
        frame_node->get_property("value")->set_any(value);
    }
};

NODE_INFO_TEMPLATE(Interpolate, Interpolate<T>, T);
TYPE_INSTANCES(InterpolateNodeInfo)

} // namespace rainynite::core::nodes
