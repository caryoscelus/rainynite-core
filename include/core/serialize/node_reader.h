/*
 *  node_reader.h - node reader implementation
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

#ifndef __CORE__SERIALIZE__NODE_READER_H__C23C4D8A
#define __CORE__SERIALIZE__NODE_READER_H__C23C4D8A

#include <core/node_info.h>
#include <core/types.h>

#include "serialize.h"

namespace core {
namespace serialize {

template <class V>
class NodeDeserializer : public Writer<V, Id> {
public:
    NodeDeserializer() :
        Writer<V, Id>()
    {}
public:
    AbstractReference get_root() const {
        return root;
    }
public:
    virtual void object_start(Id id) override {
        context.push_back(NodeContext());
        current().id = id;
    }
    virtual void object_end() override {
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
                if (auto object = dynamic_cast<AbstractNode*>(current().object.get()))
                    object->set_property(current().key, last.object);
                else
                    throw DeserializationError("Unexpected key");
            } break;
            default: break;
        }
    }
    virtual void object_value_start() override {
        await(RecordType::Value);
    }
    virtual void object_value_end() override {
        await(RecordType::Nothing);
    }
    virtual void list_start() override {
        await(RecordType::List);
    }
    virtual void list_end() override {
        await(RecordType::Nothing);
    }
    virtual void type(std::string const& s) override {
        current().object = make_node_with_name<AbstractValue>(s);
        objects.emplace(current().id, current().object);
    }
    virtual void string(std::string const& s) override {
        switch (current().awaiting) {
            case RecordType::Value: {
                auto type = current().object->get_type();
                auto value = parse_primitive_type(type, s);
                current().object->set_any(value);
            } break;
            default:
                throw DeserializationError("Unexpected string");
        }
    }
    virtual void key(std::string const& s) override {
        await(RecordType::Map);
        current().key = s;
    }
    virtual void number(double) override {
        throw DeserializationError("Unexpected number");
    }
    virtual void reference(Id id) override {
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
        Id id;
        AbstractReference object;
        RecordType awaiting;
        std::string key;
    };
private:
    void await(RecordType what) {
        current().awaiting = what;
    }
    NodeContext& current() {
        return context.back();
    }
private:
    std::vector<NodeContext> context;
    std::map<Id, AbstractReference> objects;
    AbstractReference root;
};

} // namespace serialize
} // namespace core

#endif
