/*
 *  yaml_writer.cpp - yaml serializer
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
#include <core/filters/yaml_writer.h>

using namespace fmt::literals;

namespace rainynite::core::serialize {

enum class YamlCppWriterState {
    DocumentStart,
    ObjectStart,
    TypedObject,
    AwaitValue,
    ValueObject,
    Map,
    List,
};

template <class V, typename U>
class YamlCppWriter : public Writer<V, U>, protected StateMachine<YamlCppWriterState> {
    using State = YamlCppWriterState;
public:
    YamlCppWriter(std::ostream& stream) :
        emitter(stream)
    {
        push_state(State::DocumentStart);
        emitter << YAML::BeginDoc;
    }

    void object_start(U id) override {
        emitter << YAML::Anchor(V::id_to_string(id));
        push_state(State::ObjectStart);
    }
    void object_end() override {
        if (state() == State::Map) {
            emitter << YAML::EndMap;
            pop_state(State::Map);
        } else if (state() == State::ValueObject) {
            pop_state(State::ValueObject);
        } else if (state(1) == State::Map) {
            emitter << YAML::Value << YAML::BeginMap << YAML::EndMap;
        }
        pop_state(State::TypedObject);
    }
    void object_value_start() override {
        push_state(State::AwaitValue);
    }
    void object_value_end() override {
        pop_state(State::AwaitValue);
        push_state(State::ValueObject);
    }
    void list_start() override {
        expect_state(State::AwaitValue);
        push_state(State::List);
        emitter << YAML::BeginSeq;
    }
    void list_end() override {
        emitter << YAML::EndSeq;
        pop_state(State::List);
    }
    void type(string const& s) override {
        expect_state(State::ObjectStart);
        if (!(optional_type && s.find("Value/")==0)) {
            emitter << YAML::LocalTag(s);
        }
        optional_type = false;
        set_state(State::TypedObject);
    }
    void auto_type() override {
        throw SerializationError("Not supported at the moment");
    }
    void next_type_is_optional(bool opt) override {
        optional_type = opt;
    }
    void value_string(string const& s) override {
        expect_state(State::AwaitValue);
        emitter << YAML::Value << s;
    }
    void key(string const& s) override {
        if (state() == State::TypedObject || state() == State::DocumentStart) {
            emitter << YAML::BeginMap;
            push_state(State::Map);
        }
        expect_state(State::Map);
        emitter << YAML::Key << s;
    }
    void number(double x) override {
        value_string(std::to_string(x));
    }
    void reference(U id) override {
        emitter << YAML::Alias(V::id_to_string(id));
    }

protected:
    void expect_state(State s) {
        if (s != state()) {
            throw SerializationError("YAML writer: got state {} while expecting {}."_format((int)state(), (int)s));
        }
    }

    void state_mismatch_error(State s) const override {
        throw DeserializationError("Got state {} while expecting {}"_format((int)state(), (int)s));
    }

public:
    void flush() {
        pop_state(State::DocumentStart);
        if (!emitter.good())
            throw SerializationError("YAML Emitter error: {}"_format(emitter.GetLastError()));
    }

private:
    YAML::Emitter emitter;
    bool optional_type = false;
};

} // namespace rainynite::core::serialize

namespace rainynite::core::filters {

void YamlWriter::write_document(std::ostream& output, shared_ptr<Document> document) {
    auto writer = serialize::YamlCppWriter<serialize::NodeWriter, AbstractReference>(output);
    writer.object(dynamic_pointer_cast<AbstractValue>(document));
    writer.flush();
}

} // namespace rainynite::core::filters
