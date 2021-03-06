/*  instance.cpp - explicit template instantiation
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

#include <fmt/format.h>

#include <core/node_info/macros.h>
#include <core/node/list.h>
#include <core/node_tree/io.h>
#include <core/util/type_info_macros.h>
#include <core/all_types.h>
#include <core/serialize/node_writer.h>
#include <core/time/parse.h>
#include <core/time/format.h>
#include <core/renderable.h>
#include <core/document.h>
#include <core/fs/path.h>
#include <core/color/color.h>
#include <core/util/nothing_io.h>

#include <geom_helpers/knots_io.h>
#include <geom_helpers/point_io.h>

#include <2geom/affine.h>

namespace Geom::bones {
std::ostream& operator<<(std::ostream& stream, Bone const& bone);
}


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

namespace bones {
TYPE_INFO(Bone, "Bone", [](auto&& s) {
    return Bone {};
});
} // namespace bones

} // namespace Geom

namespace std {
TYPE_INFO(string, "String", [](auto&& s) {
    return s;
});
} // namespace std

namespace rainynite::core {

TYPE_INFO_NAMED(FsPathTypeInfo, fs::Path::path_t, "FilePath", [](auto&& s) {
    return fs::Path::path_t{s};
});

TYPE_INFO(NodeTreePath, "NodePath", [](auto&& s) {
    return NodeTreePath{};
});

TYPE_INFO(BezierOutlinePath, "VariableWidthOutline", [](auto&& s) {
    return BezierOutlinePath{};
});

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

template <class T>
struct AutoValueToString :
    public ValueToString,
    class_init::Registered<AutoValueToString<T>, T, ValueToString>
{
    string operator()(any const& object) const override {
        auto value = any_cast<T>(object);
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
};

template <class T, char const* fmt_string>
struct FormatValueToString :
    public ValueToString,
    class_init::Registered<FormatValueToString<T, fmt_string>, T, ValueToString>
{
    string operator()(any const& object) const override {
        auto value = any_cast<T>(object);
        return fmt::format(fmt_string, value);
    }
};

TYPE_INSTANCES_WO_RENDERABLE_AND_CUSTOM_IO(AutoValueToString)

#define FORMAT_VALUE_SERIALIZE(Type, fmt_string) \
static char const Type##_format_string[] = fmt_string; \
template struct FormatValueToString<Type, Type##_format_string>;

FORMAT_VALUE_SERIALIZE(bool, "{}")

template <typename T>
struct FloatingValueToString :
    public ValueToString,
    class_init::Registered<FloatingValueToString<T>, T, ValueToString>
{
    string operator()(any const& object) const override {
        auto value = any_cast<T>(object);
        if (value == 0)
            return "0";
        return "{0:.{1}g}"_format(value, std::numeric_limits<T>::digits10+(int)round(fabs(log10(fabs(value)))));
    }
};
template struct FloatingValueToString<double>;

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

struct PathValueToString :
    public ValueToString,
    private class_init::Registered<
        PathValueToString,
        fs::Path::path_t,
        ValueToString
    >
{
    string operator()(any const& object) const override {
        auto v = any_cast<fs::Path::path_t>(object);
        return v.string();
    }
};

} // namespace serialize

} // namespace rainynite::core
