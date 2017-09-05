/*
 *  class_init.h - helper templates for runtime type info
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

#ifndef __CORE__CLASS_INIT_H__9F78DE78
#define __CORE__CLASS_INIT_H__9F78DE78

#include <core/std/map.h>
#include <core/std/any.h>
#include <core/std/string.h>
#include <core/std/type.h>

namespace class_init {

using rainynite::map;
using rainynite::any;
using rainynite::any_cast;
using rainynite::string;
using rainynite::Type;

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
private:
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
class Registered : public Initialized<Registered<S, T, R>> {
private:
    /**
     * Curiously-recurring pattern class argument
     */
    using Self = S;
    /**
     * Type to register
     */
    using Type = T;
    /**
     * Base type holding registered info
     */
    using RegisteredInfo = R;
public:
    static void init() {
        class_registry<R>()[typeid(T)] = new S();
    }
};

template <class S, class R>
class StringRegistered : private Initialized<StringRegistered<S, R>> {
public:
    static void init() {
        string_registry<R>()[S::name()] = new S();
    }
};

template <class S, class T, class R>
class ReverseRegistered : public Initialized<ReverseRegistered<S, T, R>> {
public:
    static void init() {
        reverse_class_registry<R>().emplace(S()(), typeid(T));
    }
};

class TypeLookupError : public std::runtime_error {
public:
    TypeLookupError(string msg="") :
        std::runtime_error("Type lookup error "+msg)
    {}
};

class RuntimeTypeError : public std::runtime_error {
public:
    RuntimeTypeError(Type type, string msg = "unknown error") :
        std::runtime_error("Runtime type ("+string(type.name())+") "+msg)
    {}
};

class UnknownTypeError : public RuntimeTypeError {
public:
    UnknownTypeError(Type type) :
        RuntimeTypeError(type, "not registered")
    {}
};

template <class T>
T& type_meta(Type type) {
    auto const& map = class_registry<T>();
    auto iter = map.find(type);
    if (iter == map.end()) {
        throw UnknownTypeError(type);
    }
    return *iter->second;
}

template <class T, class R>
R type_info(Type type) {
    auto const& t = type_meta<T>(type);
    try {
        return t();
    } catch (...) {
        throw RuntimeTypeError(type);
    }
}

template <class T, class R>
R any_info(any const& object) {
    Type type = object.type();
    auto const& t = type_meta<T>(type);
    try {
        return t(object);
    } catch (...) {
        throw RuntimeTypeError(type);
    }
}

template <class K>
Type find_type(K const& key) {
    auto const& map = reverse_class_registry<K>();
    auto iter = map.find(key);
    if (iter != map.end())
        return iter->second;
    throw TypeLookupError(key);
}

template <class T>
T& name_info(string const& name) {
    auto const& map = string_registry<T>();
    auto iter = map.find(name);
    if (iter == map.end()) {
        throw TypeLookupError(name);
    }
    return *iter->second;
}

}

#endif
