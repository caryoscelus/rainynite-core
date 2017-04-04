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
#include <core/nodes/animated.h>

#include <geom_helpers/knots.h>

namespace core {
namespace nodes {

template <class T>
class AnimatedNodeInfo : public NodeInfo, class_init::Registered<AnimatedNodeInfo<T>, Animated<T>, NodeInfo> {
public:
    virtual std::string operator()() const override {
        return "Animated<"+class_init::type_info<TypeName,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return std::make_shared<Animated<T>>();
    }
};

template class AnimatedNodeInfo<Geom::BezierKnots>;

class DoubleTypeName : public TypeName, class_init::Registered<DoubleTypeName, double, TypeName> {
public:
    virtual std::string operator()() const override {
        return "double";
    }
};

class BezierKnotsTypeName : public TypeName, class_init::Registered<BezierKnotsTypeName, Geom::BezierKnots, TypeName> {
public:
    virtual std::string operator()() const override {
        return "Knots";
    }
};

class ValueTypeNameBase {
public:
    virtual std::string operator()(boost::any const& object) const = 0;
};

class ValueTypeName : public ValueTypeNameBase, class_init::Registered<ValueTypeName, AbstractReference, ValueTypeNameBase> {
public:
    virtual std::string operator()(boost::any const& object) const {
        auto value = boost::any_cast<AbstractReference>(object);
        return class_init::type_info<TypeName, std::string>(value->get_type());
    }
};

template <typename T>
class ValueNodeInfo : public NodeInfo, class_init::Registered<ValueNodeInfo<T>, Value<T>, NodeInfo> {
public:
    virtual std::string operator()() const override {
        return "Value<"+class_init::type_info<TypeName,std::string>(typeid(T))+">";
    }
    virtual AbstractReference new_empty() const override {
        return std::make_shared<Value<T>>();
    }
};

template class ValueNodeInfo<Geom::BezierKnots>;

} // namespace nodes
} // namespace core
