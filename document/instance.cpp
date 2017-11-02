/*  instance.cpp - explicit template instantiation
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

#include <fmt/format.h>

#include <core/node_info.h>
#include <core/node/list.h>
#include <core/type_info.h>
#include <core/all_types.h>
#include <core/serialize/node_writer.h>
#include <core/time/parse.h>
#include <core/time/format.h>
#include <core/renderable.h>
#include <core/document.h>
#include <core/color/color.h>
#include <core/nothing_io.h>

#include <geom_helpers/knots_io.h>
#include <geom_helpers/point_io.h>
#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/circle.h>

#include <2geom/affine.h>

namespace Geom {

TYPE_INFO(BezierKnots, "BezierPath", [](auto&& s) {
    return parse_named_knots(s);
});

TYPE_INFO(Knot, "BezierKnot", [](auto&& s) {
    return parse_knot(s);
});

TYPE_INFO(Point, "Point", [](auto&& s) {
    return parse_point(s);
});

TYPE_INFO(NullShape, "NullShape", [](auto&& /*s*/) {
    return NullShape {};
});

TYPE_INFO(Rectangle, "Rectangle", [](auto&& s) {
    // TODO
    return Rectangle {};
});

TYPE_INFO(Circle, "Circle", [](auto&& s) {
    // TODO
    return Circle {};
});

TYPE_INFO(Affine, "Affine", [](auto&& s) {
    // TODO
    return Affine::identity();
});

} // namespace Geom

namespace std {
TYPE_INFO(string, "String", [](auto&& s) {
    return s;
});
} // namespace std

namespace rainynite::core {

namespace colors {
TYPE_INFO(Color, "Color", [](auto&& s) {
    return parse_hex(s);
});
} // namespace color

namespace nodes {

TYPE_INFO(Nothing, "Nothing", [](auto&& /*s*/) {
    return Nothing();
});

TYPE_INFO(bool, "Boolean", [](auto&& s) {
    if (s == "true")
        return true;
    else if (s == "false")
        return false;
    throw serialize::DeserializationError("Cannot parse bool from \""+s+"\"");
});

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

TYPE_INFO(Shading, "Shading", [](auto&& s) {
    // TODO
    return Shading::default_shading();
});

TYPE_INFO(Renderable, "Renderable", [](auto&& /*s*/) -> any {
    throw serialize::DeserializationError("Renderable type cannot be deserialized");
});

TYPE_INFO(Audio, "Audio", [](auto&& /*s*/) -> any {
    throw serialize::DeserializationError("Audio type cannot be deserialized");
});

TYPE_INFO(DocumentType, "Document", [](auto&& /*s*/) -> any {
    throw serialize::DeserializationError("Document type cannot be deserialized");
});

class ValueTypeInfoBase {
public:
    virtual string operator()(any const& object) const = 0;
};

class ValueTypeInfo : public ValueTypeInfoBase, class_init::Registered<ValueTypeInfo, AbstractReference, ValueTypeInfoBase> {
public:
    virtual string operator()(any const& object) const {
        auto value = any_cast<AbstractReference>(object);
        return get_primitive_type_name(value->get_type());
    }
};

NODE_INFO_TEMPLATE(Value, Value<T>, T);
NODE_INFO_TEMPLATE(List, ListValue<T>, vector<T>);

TYPE_INSTANCES_WO_RENDERABLE(ValueNodeInfo)
TYPE_INSTANCES(ListNodeInfo)

} // namespace nodes

REGISTER_NODE(UntypedListValue);

namespace serialize {

TYPE_INSTANCES_WO_RENDERABLE_AND_CUSTOM_IO(AutoValueToString)

struct AffineValueToString :
    public ValueToString,
    private class_init::Registered<
        AffineValueToString,
        Geom::Affine,
        ValueToString
    >
{
    string operator()(any const& object) const override {
        using namespace fmt::literals;
        auto v = any_cast<Geom::Affine>(object);
        return "matrix({}, {}, {}, {}, {}, {})"_format(
            v[0], v[1], v[2], v[3], v[4], v[5]
        );
    }
};

} // namespace serialize

} // namespace rainynite::core
