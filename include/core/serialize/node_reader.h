/*  serialize/node_reader.h - node reader implementation
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

#ifndef CORE_SERIALIZE_NODE_READER_H_9D80AEB0_2C37_58D3_AB90_7780A924327A
#define CORE_SERIALIZE_NODE_READER_H_9D80AEB0_2C37_58D3_AB90_7780A924327A

#include <core/node/cast.h>
#include <core/node/abstract_node.h>
#include <core/node_info.h>
#include <core/type_info.h>

#include "serialize.h"

namespace rainynite::core::serialize {

template <class V>
class NodeDeserializer : public Writer<V, NodeId> {
public:
    NodeDeserializer() :
        Writer<V, NodeId>()
    {}
public:
    AbstractReference get_root() const {
        return root;
    }
public:
    void object_start(NodeId id) override {
        context.push_back(NodeContext());
        current().id = id;
    }
    void object_end() override {
        if (context.size() == 0)
            throw DeserializationError("Unexpacted end of object");
        auto last = std::move(current());
        context.pop_back();
        if (context.size() == 0) {
            root = last.object;
        } else switch (current().awaiting) {
            case RecordType::Nothing: break;
            case RecordType::List: {
                if (auto object = dynamic_cast<AbstractListLinked*>(current().object.get()))
                    object->push_back(last.object);
                else
                    throw DeserializationError("Unexpected list element");
            } break;
            case RecordType::Map: {
                if (auto object = abstract_node_cast(current().object))
                    object->set_property(current().key, last.object);
                else
                    throw DeserializationError("Unexpected key");
            } break;
            default: break;
        }
    }
    void object_value_start() override {
        await(RecordType::Value);
    }
    void object_value_end() override {
        await(RecordType::Nothing);
    }
    void list_start() override {
        await(RecordType::List);
    }
    void list_end() override {
        await(RecordType::Nothing);
    }
    void type(string const& s) override {
        current().object = make_node_with_name<AbstractValue>(s);
        objects.emplace(current().id, current().object);
    }
    void auto_type() override {
        if (context.size() < 2) {
            throw DeserializationError("Not enough context to deduce type.");
        } else if (auto value = current(1).object) {
            if (auto list = dynamic_cast<AbstractListLinked*>(value.get())) {
                size_t id;
                if (auto node = dynamic_cast<AbstractNode*>(value.get())) {
                    id = node->get_name_id(current(1).key);
                } else {
                    id = list->link_count();
                }
                auto link_type = list->get_link_type(id);
                type("Value/"+get_primitive_type_name(link_type.only()));
            } else {
                throw DeserializationError("Cannot deduce type with non-list non-node parent.");
            }
        } else {
            throw DeserializationError("Cannot deduce type without parent node available.");
        }
    }
    void next_type_is_optional(bool /*opt*/) override {
    }
    void value_string(string const& s) override {
        switch (current().awaiting) {
            case RecordType::Value: {
                if (!current().object)
                    throw DeserializationError("Value provided before object is created");
                auto type = current().object->get_type();
                auto value = parse_primitive_type(type, s);
                current().object->set_any(value);
            } break;
            default:
                throw DeserializationError("Unexpected string");
        }
    }
    void key(string const& s) override {
        await(RecordType::Map);
        current().key = s;
    }
    void number(double) override {
        throw DeserializationError("Unexpected number");
    }
    void reference(NodeId id) override {
        // TODO: allow reference before value is constructed?
        auto target = objects.at(id);
        switch (current().awaiting) {
            case RecordType::Map: {
                auto object = dynamic_cast<AbstractNode*>(current().object.get());
                object->set_property(current().key, target);
            } break;
            case RecordType::List: {
                auto object = dynamic_cast<AbstractListLinked*>(current().object.get());
                object->push_back(target);
            } break;
            default:
                throw DeserializationError("Unexpected reference");
        }
    }
private:
    struct NodeContext {
        NodeId id;
        AbstractReference object;
        RecordType awaiting;
        string key;
    };
private:
    void await(RecordType what) {
        current().awaiting = what;
    }
    NodeContext& current() {
        return context.back();
    }
    NodeContext& current(int x) {
        return context[context.size()-x-1];
    }
private:
    vector<NodeContext> context;
    map<NodeId, AbstractReference> objects;
    AbstractReference root;
};

} // namespace rainynite::core::serialize

#endif
