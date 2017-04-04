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

#include <geom_helpers/knots.h>

namespace core {
namespace nodes {

class DoubleTypeInfo :
    public TypeInfo,
    class_init::Registered<DoubleTypeInfo, double, TypeInfo>,
    class_init::ReverseRegistered<DoubleTypeInfo, double, std::string>
{
public:
    virtual std::string operator()() const override {
        return "Real";
    }
    virtual boost::any parse_string(std::string const& s) const override {
        // TODO: check correctness & locale issues
        return std::stod(s);
    }
};

class BezierKnotsTypeInfo :
    public TypeInfo,
    class_init::Registered<BezierKnotsTypeInfo, Geom::BezierKnots, TypeInfo>,
    class_init::ReverseRegistered<BezierKnotsTypeInfo, Geom::BezierKnots, std::string>
{
public:
    virtual std::string operator()() const override {
        return "Knots";
    }
    virtual boost::any parse_string(std::string const& s) const override {
        return Geom::svg_to_knots(s);
    }
};

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
    virtual std::string operator()() const override {
        return "Value<"+class_init::type_info<TypeInfo,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return std::make_shared<Value<T>>();
    }
};

template class ValueNodeInfo<Geom::BezierKnots>;
template class ValueNodeInfo<double>;

} // namespace nodes
} // namespace core
