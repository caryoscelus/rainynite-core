/*  serialize/node_writer.h - node writer implementation
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

#ifndef CORE_SERIALIZE_NODE_WRITER_H_845A4D1B_D3C6_5562_B46D_871B10B319D6
#define CORE_SERIALIZE_NODE_WRITER_H_845A4D1B_D3C6_5562_B46D_871B10B319D6

#include <sstream>

#include <boost/uuid/uuid_io.hpp>

#include <core/node_info/node_info.h>
#include <core/node/abstract_list.h>
#include <core/node/abstract_node.h>
#include "serialize.h"

namespace rainynite::core::serialize {

class ValueToString {
public:
    virtual string operator()(any const& object) const = 0;
};

inline string value_to_string(any const& object) {
    return class_init::any_info<ValueToString, string>(object);
}

class NodeWriter {
public:
    template <class W>
    static void put_value(W& writer, AbstractReference const& object) {
        writer.value_string(value_to_string(object->static_any()));
    }

    template <class W>
    static void put_list(W& writer, AbstractReference const& object) {
        if (auto node = dynamic_cast<AbstractListLinked*>(object.get())) {
            writer.list_start();
            size_t i = 0;
            for (auto const& e : node->get_links()) {
                bool opt = node->get_link_type(i).is_only();
                writer.next_type_is_optional(opt);
                writer.object(e);
                ++i;
            }
            writer.list_end();
        }
    }

    template <class W>
    static void put_map(W& writer, AbstractReference const& object) {
        if (auto node = dynamic_pointer_cast<AbstractNode>(object)) {
            for (auto const& e : node->get_link_map()) {
                auto name = e.first;
                writer.key(name);
                bool opt = dynamic_cast<AbstractListLinked*>(node.get())->get_link_type(node->get_name_id(name)).is_only();
                writer.next_type_is_optional(opt);
                writer.object(e.second);
            }
        }
    }

    static AbstractReference get_reference(AbstractReference object) {
        if (!object)
            throw SerializationError("Received null pointer.");
        return object;
    }

    static string id_to_string(AbstractReference id) {
        return to_string(id->get_id());
    }

    static string get_type(AbstractReference object) {
        return node_name(*object);
    }

    static RecordType classify(AbstractReference object) {
        if (object->is_static())
            return RecordType::Value;
        if (dynamic_cast<AbstractNode*>(object.get()))
            return RecordType::Map;
        if (dynamic_cast<AbstractListLinked*>(object.get()))
            return RecordType::List;
        throw SerializationError("Cannot classify object");
    }
};

} // namespace rainynite::core::serialize

#endif
