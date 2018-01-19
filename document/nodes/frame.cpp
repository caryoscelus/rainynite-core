/*  frame.cpp - TimePoint / Frame node
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

template <typename T>
class Frame : public Node<TimePoint<T>> {
    DOC_STRING(
        "A (Time, Value) pair"
    )

public:
    Frame() {
        this->template init<Time>(time, {});
        this->template init<T>(value, {});
    }

    TimePoint<T> get(shared_ptr<Context> ctx) const override {
        return {
            get_time()->value(ctx),
            get_value()
        };
    }

private:
    NODE_PROPERTY(time, Time);
    NODE_PROPERTY(value, T);
};

// TODO: use generic registering mechanism

template <typename T>
class TPList : public ListValue<TimePoint<T>> {
public:
    void push_new() override {
        this->push_back(make_node<Frame<T>>());
    }
};

template <typename T>
class TimePointListNodeInfo : NODE_INFO_PARENTS(TimePointListNodeInfo<T>, TPList<T>) {
public:
    string name() const override {
        return "List/Frame/"+get_primitive_type_name<T>();
    }
    AbstractReference new_empty() const override {
        return make_shared<TPList<T>>();
    }
    AbstractReference clone(AbstractValue const& source) const override {
        return make_shared<TPList<T>>(static_cast<TPList<T> const&>(source));
    }
    Type type() const override {
        return typeid(vector<TimePoint<T>>);
    }
};

NODE_INFO_TEMPLATE(Frame, Frame<T>, TimePoint<T>);
TYPE_INSTANCES(FrameNodeInfo)
TYPE_INSTANCES(TimePointListNodeInfo)

template <typename T>
class DefaultNodeInfoTimePoint : public DefaultNodeInfoImpl<TimePoint<T>, Frame<T>> {
};

TYPE_INSTANCES(DefaultNodeInfoTimePoint)

template <typename T>
class DefaultNodeInfoTimePointList : public DefaultNodeInfoImpl<vector<TimePoint<T>>, TPList<T>> {
};

TYPE_INSTANCES(DefaultNodeInfoTimePointList)

} // namespace rainynite::core::nodes
