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
#include <list>
#include <memory>
#include <typeindex>
#include <stdexcept>

#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <core/time/time.h>

namespace core {

using Id = boost::uuids::uuid;
using Type = std::type_index;

class NodeAccessError : public std::runtime_error {
public:
    NodeAccessError(std::string const& msg) :
        std::runtime_error(msg)
    {}
};

class AbstractValue {
public:
    virtual bool is_const() const {
        return false;
    }
    virtual Type get_type() const = 0;
    virtual boost::any get_any(Time t) const = 0;
    virtual void set_any(boost::any) {
        throw NodeAccessError("Cannot set");
    }
    virtual bool can_set_any(boost::any) const {
        return false;
    }
    virtual boost::any any() const {
        throw NodeAccessError("No static value");
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
    virtual void set(T) {
        throw NodeAccessError("Cannot set");
    }
    virtual T& mod() {
        throw NodeAccessError("Cannot set");
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
    virtual void set_any(boost::any value_) override {
        set(boost::any_cast<T>(value_));
    }
    virtual bool can_set_any(boost::any value_) const override {
        return can_set() && value_.type() == typeid(T);
    }
public:
    static Type static_type() {
        return typeid(T);
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
    virtual T get(Time) const override {
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
    virtual boost::any any() const override {
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
std::shared_ptr<Value<T>> make_value(Args&&... args);

class AbstractListLinked {
public:
    virtual size_t link_count() const = 0;
    virtual std::vector<AbstractReference> get_links() const = 0;
    virtual AbstractReference get_link(size_t i) const = 0;
    virtual boost::optional<Type> get_link_type(size_t i) const = 0;
    virtual void set_link(size_t i, AbstractReference value) = 0;
    virtual void push_back(AbstractReference) {
        throw NodeAccessError("cannot push back");
    }
    virtual void push_new() {
        throw NodeAccessError("cannot push back");
    }
    virtual void remove(size_t) {
        throw NodeAccessError("cannot remove");
    }
    virtual bool is_editable_list() const {
        return false;
    }
public:
    template <class T>
    BaseReference<T> get_link_as(size_t i) const {
        return std::dynamic_pointer_cast<BaseValue<T>>(get_link(i));
    }
    template <typename T>
    void push_value(T const& value) {
        push_back(make_value<T>(value));
    }
};

using AbstractListReference = std::shared_ptr<AbstractListLinked>;

template <typename T>
class ListValue : public BaseValue<std::vector<T>>, public AbstractListLinked {
public:
    ListValue() = default;
public:
    virtual std::vector<T> get(Time t) const override {
        // TODO: caching
        std::vector<T> result;
        std::transform(
            values.begin(),
            values.end(),
            std::back_inserter(result),
            [t](auto e) {
                return e->get(t);
            }
        );
        return result;
    }
    virtual std::vector<AbstractReference> get_links() const override {
        std::vector<AbstractReference> result;
        std::transform(
            values.begin(),
            values.end(),
            std::back_inserter(result),
            [](auto e) {
                return e;
            }
        );
        return result;
    }
    virtual AbstractReference get_link(size_t i) const override {
        return values.at(i);
    }
    virtual boost::optional<Type> get_link_type(size_t) const override {
        return boost::make_optional(Type(typeid(T)));
    }
    virtual void set_link(size_t i, AbstractReference value) override {
        if (auto node = std::dynamic_pointer_cast<BaseValue<T>>(std::move(value)))
            values.at(i) = node;
    }
    virtual size_t link_count() const override {
        return values.size();
    }
    virtual void push_back(AbstractReference value) override {
        if (auto e = std::dynamic_pointer_cast<BaseValue<T>>(value)) {
            values.push_back(e);
        } else {
            //throw
        }
    }
    virtual void push_new() override {
        push_value<T>({});
    }
    virtual void remove(size_t index) override {
        values.erase(values.begin()+index);
    }
    virtual bool is_editable_list() const override {
        return true;
    }
private:
    std::vector<BaseReference<T>> values;
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

class AbstractNode : public AbstractListLinked {
public:
    AbstractReference get_property(std::string const& name) const {
        auto result = named_storage.find(name);
        if (result == named_storage.end())
            throw NodeAccessError("Unknown property "+name);
        return get_by_id(result->second);
    }
    template <typename T>
    BaseReference<T> get_property_as(std::string const& name) const {
        return std::dynamic_pointer_cast<BaseValue<T>>(get_property(name));
    }
    template <typename T>
    boost::optional<T> get_property_value(std::string const& name, Time time) const {
        try {
            return get_property_as<T>(name)->get(time);
        } catch (NodeAccessError const&) {
            return boost::none;
        } catch (...) {
            return boost::none;
        }
    }
    void set_property(std::string const& name, AbstractReference ref) {
        if (named_storage.count(name) == 0) {
            if (name[0] == '_') {
                // accept as custom attribute
                init_property(name, boost::none, ref);
            } else
                throw NodeAccessError("No such property");
        }
        get_by_id(named_storage[name]) = ref;
    }
    size_t init_property(std::string const& name, boost::optional<Type> type, AbstractReference value) {
        size_t id = link_count();
        numbered_storage.push_back(std::move(value));
        named_storage[name] = id;
        types.push_back(type);
        return id;
    }
    std::map<std::string, AbstractReference> get_link_map() const {
        std::map<std::string, AbstractReference> result;
        // TODO: use generic conversion function
        for (auto const& e : named_storage) {
            result.emplace(e.first, get_by_id(e.second));
        }
        return result;
    }
public:
    virtual std::vector<AbstractReference> get_links() const override {
        return numbered_storage;
    }
    virtual AbstractReference get_link(size_t i) const override {
        return get_by_id(i);
    }
    virtual boost::optional<Type> get_link_type(size_t i) const override {
        return types[i];
    }
    virtual void set_link(size_t i, AbstractReference value) override {
        if (auto type = get_link_type(i)) {
            if (value->get_type() != type.get())
                throw NodeAccessError("Node property type mis-match");
        }
        get_by_id(i) = value;
    }
    virtual size_t link_count() const override {
        return numbered_storage.size();
    }
private:
    AbstractReference const& get_by_id(size_t index) const {
        return numbered_storage[index];
    }
    AbstractReference& get_by_id(size_t index) {
        return numbered_storage[index];
    }
private:
    std::map<std::string, size_t> named_storage;
    std::vector<AbstractReference> numbered_storage;
    std::vector<boost::optional<Type>> types;
};

/**
 * Basic representation of any time-changeable Node
 */
template <typename T>
class Node : public BaseValue<T>, public AbstractNode {
public:
    template <typename U>
    void init(std::string const& name, U value) {
        init_property(name, boost::make_optional(Type(typeid(U))), make_value<U>(value));
    }
    template <typename U>
    void init_list(std::string const& name) {
        init_property(name, boost::make_optional(Type(typeid(std::vector<U>))), std::make_shared<ListValue<U>>());
    }
};

enum class TraverseDepth {
    Once,
    Deeper
};

template <typename T, typename F>
T traverse_once(AbstractReference root, F f, TraverseDepth depth = TraverseDepth::Once) {
    std::set<AbstractReference> traversed;
    std::list<AbstractReference> to_traverse;
    to_traverse.push_back(root);
    while (!to_traverse.empty()) {
        auto i = to_traverse.begin();
        auto node = *i;
        to_traverse.erase(i);
        if (traversed.count(node) > 0 && depth == TraverseDepth::Once)
            continue;

        if (boost::optional<T> result = f(node))
            return result.get();

        if (traversed.count(node) == 0) {
            if (auto linked_node = std::dynamic_pointer_cast<AbstractListLinked>(node)) {
                auto links = linked_node->get_links();
                std::copy_if(
                    links.begin(),
                    links.end(),
                    std::back_inserter(to_traverse),
                    [&traversed, depth](AbstractReference ref) {
                        return depth == TraverseDepth::Deeper
                            || traversed.count(ref) == 0;
                    }
                );
            }
        }
        traversed.insert(node);
    }
    return {};
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
        return this->template get_property_as<type>(#name); \
    } \
    inline void set_##name(BaseReference<type> value) { \
        this->set_property(#name, std::dynamic_pointer_cast<AbstractValue>(value)); \
    } \
private: \
    std::string name { #name }

#define NODE_STATIC_PROPERTY(name, type) \
public: \
    inline std::shared_ptr<Value<type>> get_##name() const { \
        return std::dynamic_pointer_cast<Value<type>>(this->get_property(#name)); \
    } \
    inline void set_##name(std::shared_ptr<Value<type>> value) { \
        this->set_property(#name, std::dynamic_pointer_cast<AbstractValue>(value)); \
    } \
private: \
    std::string name { #name }

#define NODE_LIST_PROPERTY(name, type) \
public: \
    inline AbstractListReference list_##name() const { \
        return std::dynamic_pointer_cast<AbstractListLinked>(this->get_property(#name)); \
    } \
NODE_PROPERTY(name, std::vector<type>)

} // namespace core

#endif
