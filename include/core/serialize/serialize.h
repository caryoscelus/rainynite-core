/*
 *  serialize.h - node tree (de)serializer
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

#ifndef __CORE__SERIALIZE__SERIALIZE_H__821FEF22
#define __CORE__SERIALIZE__SERIALIZE_H__821FEF22

#include <set>
#include <stdexcept>

#include <core/std/string.h>

namespace rainynite::core::serialize {

class SerializationError : public std::runtime_error {
public:
    SerializationError(string const& msg) :
        std::runtime_error(msg)
    {}
};

class DeserializationError : public SerializationError {
public:
    DeserializationError(string const& msg) :
        SerializationError(msg)
    {}
};

enum class RecordType {
    Nothing,
    Value,
    List,
    Map
};

class ValueWriterTemplate {
public:
    template <class W, class T>
    static void put_value(W& serializer, T const& object);

    template <class W, class T>
    static void put_list(W& serializer, T const& object);

    template <class W, class T>
    static void put_map(W& serializer, T const& object);

    template <typename U, class T>
    static U get_reference(T const& object);

    template <typename U>
    static string id_to_string(U id);

    template <class T>
    static string get_type(T const& object);

    template <class T>
    static RecordType classify(T const& object);
};

template <class V, typename U>
class Writer {
public:
    using ValueReader = V;
    using ReferenceType = U;
public:
    template <class T>
    void object(T const& object) {
        auto ref = V::get_reference(object);
        if (is_stored(ref)) {
            reference(ref);
        } else {
            store_object(ref);
            object_start(ref);
            type(V::get_type(object));
            switch (V::classify(object)) {
                case RecordType::Value:
                    object_value_start();
                    V::put_value(*this, object);
                    object_value_end();
                    break;
                case RecordType::List:
                    object_value_start();
                    V::put_list(*this, object);
                    object_value_end();
                    break;
                case RecordType::Map:
                    V::put_map(*this, object);
                    break;
                default:
                    throw SerializationError("Unexpected object type");
            }
            object_end();
        }
    }
    virtual void object_start(U id) = 0;
    virtual void object_end() = 0;
    virtual void object_value_start() = 0;
    virtual void object_value_end() = 0;
    virtual void list_start() = 0;
    virtual void list_end() = 0;
    virtual void type(string const& s) = 0;
    virtual void value_string(string const& s) = 0;
    virtual void key(string const& s) = 0;
    virtual void number(double x) = 0;
    virtual void reference(U id) = 0;
private:
    std::set<U> storage;
    bool is_stored(U id) {
        return storage.count(id);
    }
    void store_object(U id) {
        storage.insert(id);
    }
};

} // namespace rainynite::core::serialize

#endif
