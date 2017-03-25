/*
 *  node.h - Node system
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

#ifndef __CORE__NODE_H__CC4B44C6
#define __CORE__NODE_H__CC4B44C6

#include <map>
#include <memory>

#include <boost/any.hpp>

#include <core/time/time.h>

namespace core {

class AbstractValue {
public:
    virtual boost::any get_any(Time t) const = 0;
    virtual void set_any(boost::any value_) {
        throw "Cannot set";
    }
    virtual bool can_set_any(boost::any value_) const {
        return false;
    }
};

template <typename T>
class BaseValue : public AbstractValue {
public:
    virtual T get(Time t) const = 0;
    virtual void set(T value_) {
        throw "Cannot set";
    }
    virtual bool can_set() const {
        return false;
    }
public:
    virtual boost::any get_any(Time t) const override {
        return get(t);
    }
    virtual void set(boost::any value_) {
        set(boost::any_cast<T>(value_));
    }
    virtual bool can_set_any(boost::any value_) const override {
        return can_set() && value_.type() == typeid(T);
    }
};

using AbstractReference = std::shared_ptr<AbstractValue>;
template <typename T>
using BaseReference = std::shared_ptr<BaseValue<T>>;

template <typename T>
class Value : public BaseValue<T> {
public:
    explicit Value(T value_) :
        value(value_)
    {}
public:
    virtual T get(Time t) const override {
        return value;
    }
    virtual void set(T value_) override {
        value = value_;
    }
    virtual bool can_set() const override {
        return true;
    }
private:
    T value;
};

class AbstractNode {
public:
    AbstractReference get_property(std::string const& name) const {
        return *(named_storage.at(name));
    }
    void set_property(std::string const& name, AbstractReference ref) {
        if (named_storage.count(name) == 0) {
            throw "No such property";
        }
        *(named_storage[name]) = ref;
    }
    void init_property(std::string const& name, AbstractReference* ref) {
        named_storage[name] = ref;
    }
private:
    std::map<std::string, AbstractReference*> named_storage;
};

template <typename T>
class Property {
public:
    Property(std::string const& name_, std::string const& type_name_):
        name(name_),
        type_name(type_name_)
    {}
    virtual ~Property()
    {}
public:
    inline T get() const {
        return value;
    }
    inline void set(T value_) {
        value = value_;
    }
protected:
    std::string name;
    std::string type_name;
    T value;
};

class NodeProperty : public Property<AbstractReference> {
public:
    template <typename... Ts>
    NodeProperty(Ts... args) :
        Property(std::forward<Ts>(args)...)
    {}
public:
    void init(AbstractNode* parent, AbstractReference value_) {
        set(value_);
        parent->init_property(name, &value);
    }
};

/**
 * Basic representation of any time-changeable Node
 */
template <typename T>
class Node : public BaseValue<T>, public AbstractNode {
};

#define NODE_PROPERTY(name, type) \
public: \
    inline BaseReference<type> get_##name() const { \
        return std::static_pointer_cast<BaseValue<type>>(name.get()); \
    } \
    inline void set_##name(BaseReference<type> value) { \
        name.set(std::static_pointer_cast<AbstractValue>(value)); \
    } \
private: \
    NodeProperty name { #name, #type }

using Real = double;

} // namespace core

#endif
