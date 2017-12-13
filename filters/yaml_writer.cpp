/*  filters/yaml_writer.cpp - yaml serializer
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

#include <fmt/format.h>

#include <yaml-cpp/emitter.h>

#include <core/document.h>
#include <core/state_machine.h>
#include <core/serialize/node_writer.h>
#include <core/node_info.h>
#include <core/node_tree_traverse.h>
#include <core/filters/yaml_writer.h>

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
    YamlCppWriter(std::ostream& stream) :
        emitter(stream)
    {
    }

    void serialize(shared_ptr<Document> document) {
        push_state(State::DocumentStart);
        emitter << YAML::BeginDoc;
        tree = document->get_tree();
        traverse_tree(tree);
    }

    bool object_start() override {
        if (state() == State::Map) {
            emitter << YAML::Key << current().key;
        }

        auto const& node = current().node;
        if (current().count > 0) {
            write_link(node);
            return false;
        }
        write_anchor(node);

        auto type_s = node_name(*node);
        if (!(current().type.is_only() && type_s.find("Value/")==0)) {
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

    void object_end() override {
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
    YAML::Emitter emitter;
    shared_ptr<NodeTree> tree;
};


void YamlWriter::write_document(std::ostream& output, shared_ptr<Document> document) {
    auto writer = YamlCppWriter(output);
    writer.serialize(document);
    writer.flush();
}

} // namespace rainynite::core::filters
