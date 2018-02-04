/*  filters/yaml_writer.cpp - yaml serializer
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

#include <fmt/format.h>

#include <yaml-cpp/emitter.h>

#include <core/document.h>
#include <core/util/state_machine.h>
#include <core/serialize/node_writer.h>
#include <core/node_info/node_info.h>
#include <core/node_tree/traverse.h>
#include <core/filters/writer.h>

using namespace fmt::literals;

namespace rainynite::core::filters {

enum class YamlCppWriterState {
    DocumentStart,
    Map,
    List,
};

class YamlCppWriter :
    public TreeTraverser,
    protected StateMachine<YamlCppWriterState>
{
    using State = YamlCppWriterState;
public:
    YamlCppWriter(std::ostream& stream, shared_ptr<AbstractDocument> document) :
        TreeTraverser(*document->get_tree()),
        emitter(stream)
    {
    }

    void serialize() {
        push_state(State::DocumentStart);
        emitter << YAML::BeginDoc;
        traverse_tree();
    }

    bool object_start(NodeTree::Index index) override {
        if (state() == State::Map) {
            emitter << YAML::Key << tree.link_key(index);
        }

        auto node = tree.get_node(index);
        if (tree.get_node_count(node) > 1) {
            local_count.emplace(node, 0);
            if (local_count.at(node) > 0) {
                write_link(node);
                return false;
            }
            write_anchor(node);
            ++local_count[node];
        }

        auto type_s = node_name(*node);
        if (!(tree.type_of(index).is_only() && type_s.find("Value/")==0)) {
            emitter << YAML::LocalTag(type_s);
        }

        switch (serialize::NodeWriter::classify(node)) {
            case serialize::RecordType::Value: {
                emitter << YAML::Value << serialize::value_to_string(node->static_any());
                return false;
            }
            case serialize::RecordType::Map: {
                push_state(State::Map);
                emitter << YAML::BeginMap;
                return true;
            }
            case serialize::RecordType::List: {
                push_state(State::List);
                emitter << YAML::BeginSeq;
                return true;
            }
            default:
                throw serialize::SerializationError("universe imploded");
        }
    }

    void object_end(NodeTree::Index) override {
        switch (state()) {
            case State::List: {
                emitter << YAML::EndSeq;
            } break;
            case State::Map: {
                emitter << YAML::EndMap;
            } break;
            default:
                state_mismatch_error(State::Map);
        }
        pop_state();
    }

    void flush() {
        pop_state(State::DocumentStart);
        if (!emitter.good())
            throw serialize::SerializationError("YAML Emitter error: {}"_format(emitter.GetLastError()));
    }

private:
    void state_mismatch_error(State s) const override {
        throw serialize::SerializationError("Got state {} while expecting {}"_format((int)state(), (int)s));
    }

    void write_anchor(AbstractReference node) {
        emitter << YAML::Anchor(id_to_string(*node));
    }

    void write_link(AbstractReference node) {
        emitter << YAML::Alias(id_to_string(*node));
    }

private:
    using NodeMap = map<weak_ptr<AbstractValue>, size_t, std::owner_less<weak_ptr<AbstractValue>>>;
    NodeMap local_count;
    YAML::Emitter emitter;
};


class YamlWriter : FILTER_WRITE(YamlWriter) {
    FILTER_NAME("yaml")

public:
    void write_document(std::ostream& output, shared_ptr<AbstractDocument> document) const override {
        auto writer = YamlCppWriter(output, document);
        writer.serialize();
        writer.flush();
    }
};


} // namespace rainynite::core::filters
