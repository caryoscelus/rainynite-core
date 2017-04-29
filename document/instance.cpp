/*
 *  instance.cpp - explicit template instantiation
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
#include <core/types.h>
#include <core/serialize/node_writer.h>
#include <core/time/parse.h>
#include <core/time/format.h>
#include <core/renderable.h>
#include <core/document.h>
#include <core/color.h>

#include <geom_helpers/knots_io.h>
#include <geom_helpers/point_io.h>

namespace Geom {

TYPE_INFO(BezierKnots, "BezierPath", [](auto&& s) {
    return parse_named_knots(s);
});

TYPE_INFO(Point, "Point", [](auto&& s) {
    return parse_point(s);
});

} // namespace Geom

namespace std {
TYPE_INFO(string, "String", [](auto&& s) {
    return s;
});
} // namespace std

namespace core {

namespace colors {
TYPE_INFO(Color, "Color", [](auto&& s) {
    return parse_hex(s);
});
} // namespace color

namespace nodes {

TYPE_INFO(double, "Real", [](auto&& s) {
    // TODO: check correctness & locale issues
    return std::stod(s);
});

TYPE_INFO(TimePeriod, "TimePeriod", [](auto&& s) {
    return parse_time_period(s);
});

TYPE_INFO(Time, "Time", [](auto&& s) {
    return parse_time(s);
});

TYPE_INFO(Renderable, "Renderable", [](auto&&) -> boost::any {
    throw serialize::DeserializationError("Renderable type cannot be deserialized");
});

TYPE_INFO(DocumentType, "Document", [](auto&&) -> boost::any {
    throw serialize::DeserializationError("Renderable type cannot be deserialized");
});

class ValueTypeInfoBase {
public:
    virtual std::string operator()(boost::any const& object) const = 0;
};

class ValueTypeInfo : public ValueTypeInfoBase, class_init::Registered<ValueTypeInfo, AbstractReference, ValueTypeInfoBase> {
public:
    virtual std::string operator()(boost::any const& object) const {
        auto value = boost::any_cast<AbstractReference>(object);
        return class_init::type_info<TypeInfo, std::string>(value->get_type());
    }
};

template <typename T>
class ValueNodeInfo :
    public NodeInfo,
    class_init::Registered<ValueNodeInfo<T>, Value<T>, NodeInfo>,
    class_init::ReverseRegistered<ValueNodeInfo<T>, Value<T>, std::string>
{
public:
    virtual std::string name() const override {
        return "Value<"+class_init::type_info<TypeInfo,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return std::make_shared<Value<T>>();
    }
    virtual Type type() const override {
        return typeid(T);
    }
};

template <typename T>
class ListValueNodeInfo :
    public NodeInfo,
    class_init::Registered<ListValueNodeInfo<T>, ListValue<T>, NodeInfo>,
    class_init::ReverseRegistered<ListValueNodeInfo<T>, ListValue<T>, std::string>
{
public:
    virtual std::string name() const override {
        return "List<"+class_init::type_info<TypeInfo,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return std::make_shared<ListValue<T>>();
    }
    virtual Type type() const override {
        return typeid(std::vector<T>);
    }
};

template class ValueNodeInfo<Geom::BezierKnots>;
template class ValueNodeInfo<Geom::Point>;
template class ValueNodeInfo<double>;
template class ValueNodeInfo<Time>;
template class ValueNodeInfo<TimePeriod>;
template class ValueNodeInfo<colors::Color>;
template class ValueNodeInfo<std::string>;
// template class ListValueNodeInfo<Renderable>; - no renderable values possible

template class ListValueNodeInfo<Geom::BezierKnots>;
template class ListValueNodeInfo<Geom::Point>;
template class ListValueNodeInfo<double>;
template class ListValueNodeInfo<Time>;
template class ListValueNodeInfo<TimePeriod>;
template class ListValueNodeInfo<colors::Color>;
template class ListValueNodeInfo<std::string>;
template class ListValueNodeInfo<Renderable>;

} // namespace nodes

namespace serialize {

template class AutoValueToString<Geom::BezierKnots>;
template class AutoValueToString<Geom::Point>;
template class AutoValueToString<double>;
template class AutoValueToString<Time>;
template class AutoValueToString<TimePeriod>;
template class AutoValueToString<colors::Color>;
template class AutoValueToString<std::string>; // TODO: remove streams overhead

} // namespace serialize

} // namespace core
