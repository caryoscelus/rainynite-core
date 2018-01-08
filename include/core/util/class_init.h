/*  util/class_init.h - helper templates for runtime type info
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

#ifndef CORE_CLASS_INIT_H_0DD6B78B_E387_5F16_9B97_E6E7F8FCBE57
#define CORE_CLASS_INIT_H_0DD6B78B_E387_5F16_9B97_E6E7F8FCBE57

#include <core/std/map.h>
#include <core/std/any.h>
#include <core/std/string.h>
#include <core/std/vector.h>
#include <core/std/type.h>

namespace class_init {

using rainynite::map;
using rainynite::any;
using rainynite::any_cast;
using rainynite::string;
using rainynite::vector;
using rainynite::Type;

/**
 * Type lookup error.
 *
 * Thrown when the key doesn't have a type associated with it.
 */
struct TypeLookupError : public std::runtime_error {
    TypeLookupError(string msg="") :
        std::runtime_error("Type lookup error "+msg)
    {}
};

/**
 * Runtime type error.
 *
 * Thrown by various runtime type info functions when requested type
 * isn't found.
 */
struct RuntimeTypeError : public std::runtime_error {
    RuntimeTypeError(Type type, string msg = "unknown error") :
        std::runtime_error("Runtime type ("+string(type.name())+") "+msg)
    {}
};

/**
 * Runtime type error: type is unknown
 */
struct UnknownTypeError : public RuntimeTypeError {
    UnknownTypeError(Type type) :
        RuntimeTypeError(type, "not registered")
    {}
};

/**
 * Automatic class initialization helper.
 *
 * To use this class, inherit from it in a "curiously returning template
 * pattern" (e.g. `class Foo : Initialized < Foo >`) and define a public
 * static init() function - it will be launch on init (i.e. on static
 * initialization stage - before main() in executable or on load of dynamic
 * library.
 */
template <class T>
class Initialized {
    struct Init {
        Init() {
            T::init();
        }
    };
    static Init instance;
    template <Init*> struct RR {};
    static RR<&instance> r_insatnce;
};

template <typename T>
typename Initialized<T>::Init Initialized<T>::instance;

template <class T>
map<Type, T*>& class_registry() {
    static map<Type, T*> instance;
    return instance;
}

template <class T>
map<T, Type>& reverse_class_registry() {
    static map<T, Type> instance;
    return instance;
}

template <class T>
map<string, T*>& string_registry() {
    static map<string, T*> instance;
    return instance;
}

template <class T>
vector<T*>& class_list_registry() {
    static vector<T*> instance;
    return instance;
}

/**
 * Automatic class registration helper.
 *
 * This class template (along with class_registry() template) allows to
 * create automatically-registered binding between a type and some value
 * which can then be retrieved in runtime.
 *
 * Usage:
 * - Create an (abstract) Base class which will hold required type
 *   information and interface to retrieve it
 * - For each type, create public Base SubClass which also inherits from
 *   Registered< SubClass, TargetType, Base >
 * - Retrieve information by querying class_registry() with runtime type
 *   information (type_index or type_info which converts to it).
 */
template <class S, class T, class R>
struct Registered : public Initialized<Registered<S, T, R>> {
    /// Curiously-recurring pattern class argument
    using Self = S;
    /// Type to register
    using Type = T;
    /// Base type holding registered info
    using RegisteredInfo = R;
    static void init() {
        class_registry<R>()[typeid(T)] = new S();
    }
};

/**
 * Automatic string-to-class registration helper.
 *
 * This template allows associating strings with classes (quite
 * often those classes are factories).
 */
template <class S, class R>
struct StringRegistered : private Initialized<StringRegistered<S, R>> {
    /**
     * Curiously-recurring pattern class argument
     *
     * Should have public static string name() which result is
     * used for registering.
     */
    using Self = S;
    /// Base class of registered info
    using RegisteredInfo = R;
    static void init() {
        string_registry<R>()[S::name()] = new S();
    }
};

/**
 * Automatic register class list helper.
 */
template <class S, class R>
struct ListRegistered : private Initialized<ListRegistered<S, R>> {
    using Self = S;
    static void init() {
        class_list_registry<R>().push_back(new S());
    }
};

/**
 * Automatic "reverse" class registry helper.
 *
 * Usage:
 *
 * - For each registered type, create a subclass of this template with
 *   appropriate template arguments (Self, Type, Key).
 * - That subclass should have operator()() that returns unique Key for
 *   the Type.
 */
template <class S, class T, class K>
struct ReverseRegistered : public Initialized<ReverseRegistered<S, T, K>> {
    /**
     * Curiously-recurring pattern class argument
     *
     * Should have R operator()() that returns desired key
     */
    using Self = S;
    /// Type to register
    using Type = T;
    /// Key type
    using Key = K;
    static void init() {
        reverse_class_registry<K>().emplace(S()(), typeid(T));
    }
};

/// Get type info (of type `T`) for type `type`
template <class T>
T& type_meta(Type type) {
    auto const& class_map = class_registry<T>();
    auto iter = class_map.find(type);
    if (iter == class_map.end()) {
        throw UnknownTypeError(type);
    }
    return *iter->second;
}

/**
 * Get type info for type `type`
 *
 * Since T (type info class) must be unique per type info set, even if it's
 * only purpose is to store single value caller still needs to obtain that
 * object and get value from it. This template allows to get required value
 * (of type R) directly by calling operator() on T.
 */
template <class T, class R, typename... Args>
R type_info(Type type, Args&&... args) {
    auto const& t = type_meta<T>(type);
    return t(std::forward<Args>(args)...);
}

/**
 * Get type_info for `any` object.
 *
 * This template is similar to type_info, with the difference that it accepts
 * `any` instance and forwards it to operator() of T.
 */
template <class T, class R>
R any_info(any const& object) {
    return type_info<T, R>(object.type(), object);
}

/// Find type that is registered using reverse_class_registry
template <class K>
Type find_type(K const& key) {
    auto const& class_map = reverse_class_registry<K>();
    auto iter = class_map.find(key);
    if (iter != class_map.end())
        return iter->second;
    throw TypeLookupError(key);
}

/// Get name info from string_registry
template <class T>
T& name_info(string const& name) {
    auto const& class_map = string_registry<T>();
    auto iter = class_map.find(name);
    if (iter == class_map.end()) {
        // TODO: should we really throw TypeLookupError here?
        throw TypeLookupError(name);
    }
    return *iter->second;
}

} // namespace class_init

#endif
