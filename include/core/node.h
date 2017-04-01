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
#include <set>
#include <memory>
#include <typeindex>

#include <boost/any.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <core/time/time.h>

namespace core {

using Id = boost::uuids::uuid;
using Type = std::type_index;

class AbstractValue {
public:
    virtual bool is_const() const {
        return false;
    }
    virtual Type get_type() const = 0;
    virtual boost::any get_any(Time t) const = 0;
    virtual void set_any(boost::any value_) {
        throw "Cannot set";
    }
    virtual bool can_set_any(boost::any value_) const {
        return false;
    }
    virtual boost::any any() const {
        throw "No static value";
    }
    Id get_id() {
        return id;
    }
    void set_id(Id id_) {
        id = id_;
    }
    void new_id() {
        id = boost::uuids::random_generator()();
    }
private:
    Id id;
};

template <typename T>
class BaseValue : public AbstractValue {
public:
    virtual T get(Time t) const = 0;
    virtual void set(T value_) {
        throw "Cannot set";
    }
    virtual T& mod() {
        throw "Cannot set";
    }
    virtual bool can_set() const {
        return false;
    }
public:
    virtual Type get_type() const override {
        return typeid(T);
    }
    virtual boost::any get_any(Time t) const override {
        return get(t);
    }
    virtual void set_any(boost::any value_) {
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
    virtual bool is_const() const override {
        return true;
    }
    virtual T get(Time t) const override {
        return value;
    }
    virtual void set(T value_) override {
        value = value_;
    }
    virtual T& mod() override {
        return value;
    }
    virtual bool can_set() const override {
        return true;
    }
    virtual boost::any any() const {
        return value;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        value = T(std::forward<Args>(args)...);
    }

private:
    T value;
};

template <typename T, typename... Args>
std::shared_ptr<Value<T>> make_value(Args&&... args) {
    auto r = std::make_shared<Value<T>>();
    r->new_id();
    r->emplace(std::forward<Args>(args)...);
    return r;
}

template <class T>
std::shared_ptr<T> make_node() {
    auto r = std::make_shared<T>();
    r->new_id();
    return r;
}

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
    void init_property(std::string const& name, AbstractReference* ref_p) {
        named_storage[name] = ref_p;
        numbered_storage.push_back(ref_p);
    }
    std::map<std::string, AbstractReference> get_link_map() const {
        std::map<std::string, AbstractReference> result;
        // TODO: use generic conversion function
        for (auto const& e : named_storage) {
            result.emplace(e.first, *e.second);
        }
        return result;
    }
    std::vector<AbstractReference> get_links() const {
        std::vector<AbstractReference> result;
        for (auto e : numbered_storage) {
            result.push_back(*e);
        }
        return result;
    }
    size_t link_count() const {
        return numbered_storage.size();
    }
private:
    std::map<std::string, AbstractReference*> named_storage;
    std::vector<AbstractReference*> numbered_storage;
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
    inline T& mod() {
        return value;
    }
    inline void set(T value_) {
        value = value_;
    }
    inline std::string get_name() const {
        return name;
    }
protected:
    std::string name;
    std::string type_name;
    T value;
};

using NodeProperty = Property<AbstractReference>;

/**
 * Basic representation of any time-changeable Node
 */
template <typename T>
class Node : public BaseValue<T>, public AbstractNode {
public:
    template <typename U>
    void init(NodeProperty& prop, U value) {
        prop.set(make_value<U>(value));
        init_property(prop.get_name(), &(prop.mod()));
    }
};

void traverse_once(AbstractReference root, auto f) {
    std::set<AbstractReference> traversed;
    std::set<AbstractReference> to_traverse;
    to_traverse.insert(root);
    while (!to_traverse.empty()) {
        auto i = to_traverse.begin();
        auto node = *i;
        to_traverse.erase(i);
        f(node);
        auto linked_node = std::dynamic_pointer_cast<AbstractNode>(node);
        if (linked_node) {
            auto links = linked_node->get_links();
            std::copy_if(
                links.begin(),
                links.end(),
                std::inserter(to_traverse, to_traverse.end()),
                [&traversed](AbstractReference ref) {
                    return traversed.count(ref) == 0;
                }
            );
        }
        traversed.insert(node);
    }
}

/*
 * NOTE: static_pointer_cast might be unsafe (silently ignoring when node is
 * being used incorrectly), so using dynamic_pointer_cast (which would lead
 * to instant segfault instead) for now. However, it might inflict too much
 * performance hit since getter can be used very frequently.
 * Because of this, we might revert to static or even reinterpret cast in
 * the future...
 */
#define NODE_PROPERTY(name, type) \
public: \
    inline BaseReference<type> get_##name() const { \
        return std::dynamic_pointer_cast<BaseValue<type>>(name.get()); \
    } \
    inline void set_##name(BaseReference<type> value) { \
        name.set(std::dynamic_pointer_cast<AbstractValue>(value)); \
    } \
private: \
    NodeProperty name { #name, #type }

} // namespace core

#endif
