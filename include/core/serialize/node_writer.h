/*
 *  node_writer.h - node writer implementation
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

#ifndef __CORE__SERIALIZE__NODE_WRITER__50EE0930
#define __CORE__SERIALIZE__NODE_WRITER__50EE0930

#include <core/node_info.h>
#include "serialize.h"

namespace core {
namespace serialize {

class ValueToString {
public:
    virtual std::string operator()(boost::any const& object) const = 0;
};

template <class T>
class AutoValueToString : public ValueToString, class_init::Registered<AutoValueToString<T>, T, ValueToString> {
public:
    virtual std::string operator()(boost::any const& object) const override {
        auto value = boost::any_cast<T>(object);
        return std::to_string(value);
    }
};

class NodeWriter {
public:
    template <class W>
    static void put_value(W& writer, AbstractReference const& object) {
        writer.string(class_init::any_info<ValueToString, std::string>(object->any()));
    }

    template <class W>
    static void put_list(W& writer, AbstractReference const& object) {
        //
    }

    template <class W>
    static void put_map(W& writer, AbstractReference const& object) {
        if (auto node = std::dynamic_pointer_cast<AbstractNode>(object)) {
            for (auto const& e : node->get_link_map()) {
                writer.key(e.first);
                writer.object(e.second);
            }
        }
    }

    static AbstractReference get_reference(AbstractReference object) {
        return object;
    }

    static std::string id_to_string(AbstractReference id) {
        return to_string(id->get_id());
    }

    static std::string get_type(AbstractReference object) {
        return node_name(object);
    }

    static serialize::RecordType classify(AbstractReference object) {
        if (object->is_const())
            return serialize::RecordType::Value;
        return serialize::RecordType::Map;
    }
};

} // namespace serialize
} // namespace core

#endif