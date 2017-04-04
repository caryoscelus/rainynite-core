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

#ifndef __CORE__SERIALIZE_H__821FEF22
#define __CORE__SERIALIZE_H__821FEF22

#include <set>
#include <ostream>

namespace core {
namespace serialize {

enum class RecordType {
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
    static std::string id_to_string(U id);

    template <class T>
    static std::string get_type(T const& object);

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
    virtual void type(std::string const& s) = 0;
    virtual void string(std::string const& s) = 0;
    virtual void key(std::string const& s) = 0;
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

template <class V, typename U>
class DumbJsonWriter : public Writer<V, U> {
public:
    DumbJsonWriter(std::ostream& stream_) :
        stream(stream_)
    {}
public:
    virtual void object_start(U id) override {
        element();
        write("{");
        prev_element = false;
        key("UID");
        string(V::id_to_string(id));
    }
    virtual void object_end() override {
        write("}");
        prev_element = true;
    }
    virtual void object_value_start() override {
        key("VALUE");
    }
    virtual void object_value_end() override {
    }
    virtual void list_start() override {
        write("[");
        prev_element = false;
    }
    virtual void list_end() override {
        write("]");
        prev_element = true;
    }
    virtual void type(std::string const& s) override {
        key("TYPE");
        write_string(s);
        element();
    }
    virtual void string(std::string const& s) override {
        element();
        write_string(s);
    }
    virtual void key(std::string const& s) override {
        element();
        write_string(s);
        write(":");
        prev_element = false;
    }
    virtual void number(double x) override {
        element();
        write(std::to_string(x));
    }
    virtual void reference(U id) override {
        string(V::id_to_string(id));
    }
private:
    void element() {
        if (prev_element)
            write(",");
        prev_element = true;
    }
    void write(std::string const& s) {
        stream << s;
    }
    void write_string(std::string const& s) {
        write("\"");
        write(s);
        write("\"");
    }
private:
    std::ostream& stream;
    bool prev_element = false;
};

} // namespace serialize
} // namespace core

#endif
