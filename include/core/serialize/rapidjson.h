/*
 *  rapid_json.h - JSON (de)serialize via RapidJSON
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

#ifndef __CORE__SERIALIZE__RAPIDJSON_H__38642728
#define __CORE__SERIALIZE__RAPIDJSON_H__38642728

#include <iostream>

#include <boost/uuid/string_generator.hpp>

#include <rapidjson/reader.h>

#include "serialize.h"

namespace core {
namespace serialize {

boost::uuids::string_generator s_to_id;

template <class W>
class RJSONWrapper {
private:
    enum class Status {
        Empty,
        Type,
        UID,
        Value,
        ObjectStart,
    };
public:
    bool Null() {
        std::cerr << "Null" << std::endl;
        throw DeserializationError("Unexpected null");
    }
    bool Bool(bool) {
        std::cerr << "Bool" << std::endl;
        throw DeserializationError("Unexpected bool");
    }
    bool Int(int) {
        std::cerr << "Int" << std::endl;
        throw DeserializationError("Unexpected int");
    }
    bool Uint(unsigned) {
        throw DeserializationError("Unexpected uint");
    }
    bool Int64(int64_t) {
        throw DeserializationError("Unexpected int64");
    }
    bool Uint64(uint64_t u) {
        throw DeserializationError("Unexpected uint64");
    }
    bool Double(double d) {
        throw DeserializationError("Unexpected double");
    }
    bool RawNumber(char const* str, size_t length, bool copy) { 
        std::cerr << "RawNumber" << std::endl;
        throw DeserializationError("Unexpected number");
    }
    bool String(char const* str, size_t length, bool copy) { 
        std::cerr << "String" << std::endl;
        auto s = std::string(str, length);
        switch (status) {
            case Status::Type: {
                writer.type(s);
                status = Status::Empty;
            } break;
            case Status::UID: {
                auto id = s_to_id(s);
                writer.object_start(id);
            } break;
            case Status::Value: {
                writer.string(s);
            } break;
            case Status::Empty: {
                auto id = s_to_id(s);
                writer.reference(id);
            } break;
            default:
                std::cerr << s << std::endl;
                std::cerr << (int)status << std::endl;
                throw DeserializationError("Unexpected status");
        }
        return true;
    }
    bool StartObject() {
        std::cerr << "StartObject" << std::endl;
        status = Status::ObjectStart;
        return true;
    }
    bool Key(char const* str, size_t length, bool copy) {
        std::cerr << "Key" << std::endl;
        auto key = std::string(str, length);
        if (key == "TYPE") {
            status = Status::Type;
        } else if (key == "VALUE") {
            writer.object_value_start();
            status = Status::Value;
        } else if (key == "UID") {
            // relying on UID being first element in JSON object
            // this should NOT be relied on as JSON does not preserve order
            std::cerr << "UID" << std::endl;
            std::cerr << (int)status << std::endl;
            if (status != Status::ObjectStart)
                throw DeserializationError("Unexpected UID");
            status = Status::UID;
        } else {
            writer.key(key);
        }
        std::cerr << "exit key" << std::endl;
        return true;
    }
    bool EndObject(size_t) {
        std::cerr << "EndObject" << std::endl;
        status = Status::Empty;
        writer.object_end();
        return true;
    }
    bool StartArray() {
        std::cerr << "StartArray" << std::endl;
        writer.list_start();
        return true;
    }
    bool EndArray(size_t) {
        std::cerr << "EndArray" << std::endl;
        writer.list_end();
        return true;
    }
public:
    W const& get_node_reader() const {
        return writer;
    }
private:
    W writer;
    Status status;
};

} // namespace serialize
} // namespace core

#endif
