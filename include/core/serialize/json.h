/*
 *  json.h - JSON (de)serialize wrappers
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

#ifndef __CORE__SERIALIZE__JSON_H__9386BE58
#define __CORE__SERIALIZE__JSON_H__9386BE58

#include "serialize.h"

namespace core {
namespace serialize {

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