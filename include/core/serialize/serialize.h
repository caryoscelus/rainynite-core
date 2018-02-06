/*  serialize.h - node tree (de)serializer
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

#ifndef CORE_SERIALIZE_SERIALIZE_H_D7B6BA59_030E_5601_AA64_88A95D447ADB
#define CORE_SERIALIZE_SERIALIZE_H_D7B6BA59_030E_5601_AA64_88A95D447ADB

#include <core/std/set.h>
#include <core/std/string.h>

#include "exceptions.h"

namespace rainynite::core::serialize {

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

template <typename U>
class Writer {
public:
    using ReferenceType = U;
public:
    virtual void object_start(U id) = 0;
    virtual void object_end() = 0;
    virtual void object_value_start() = 0;
    virtual void object_value_end() = 0;
    virtual void list_start() = 0;
    virtual void list_end() = 0;

    /// Write current node type (called after object_start)
    virtual void type(string const& s) = 0;

    /// Infer type from key name or list type or throw if it's impossible
    virtual void auto_type() = 0;

    /// Marks whether next supplied type can be ignored
    virtual void next_type_is_optional(bool opt) = 0;

    virtual void value_string(string const& s) = 0;
    virtual void key(string const& s) = 0;
    virtual void number(double x) = 0;
    virtual void reference(U id) = 0;

    virtual void set_name(string const& name) = 0;
    virtual void set_enabled(bool enabled) = 0;

private:
    set<U> storage;
    bool is_stored(U id) {
        return storage.count(id);
    }
    void store_object(U id) {
        storage.insert(id);
    }
};

} // namespace rainynite::core::serialize

#endif
