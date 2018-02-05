/*  yaml_reader.cpp - YAML document reader
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

#include <iostream>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/random_generator.hpp>

#include <fmt/format.h>

#include <yaml-cpp/eventhandler.h>
#include <yaml-cpp/parser.h>

#include <core/document.h>
#include <core/node_info/node_info.h>
#include <core/util/state_machine.h>
#include <core/filters/reader.h>
#include <core/serialize/node_reader.h>

using namespace fmt::literals;

namespace rainynite::core::serialize {

using s_to_id = string;
inline string random_id() {
    boost::uuids::random_generator gen;
    return to_string(gen());
}

enum class YamlCppWrapperState {
    Map,
    Value,
    List
};

template <class W>
class YamlCppWrapper : public YAML::EventHandler, protected StateMachine<YamlCppWrapperState> {
    using Mark = YAML::Mark;
    using anchor_t = YAML::anchor_t;
    using EmitterStyle = YAML::EmitterStyle;
    using State = YamlCppWrapperState;
public:
    void OnDocumentStart(Mark const&) override {
    }
    void OnDocumentEnd() override {
    }

    void OnNull(Mark const&, anchor_t) override {
        std::cerr << "Got null\n";
        throw DeserializationError("null is not supported (yet)");
    }
    void OnAlias(Mark const&, anchor_t anchor_id) override {
        std::cerr << "Got alias\n";
        auto id = s_to_id(anchors[anchor_id - 1]);
        writer.reference(id);
        update_map_state();
    }
    void OnScalar(Mark const&, string const& tag, anchor_t, string const& value) override {
        std::cerr << "Got tag {} with value {}\n"_format(tag, value);
        switch (state()) {
            case State::Map: {
                if (tag != "?")
                    throw DeserializationError("Got key {} with a tag {}"_format(value, tag));
                if (!anchor.empty())
                    throw DeserializationError("Got key {} with an anchor {}"_format(value, anchor));
                writer.key(value);
                set_state(State::Value);
            } break;
            case State::Value:
                set_state(State::Map);
                [[fallthrough]];
            case State::List:
                new_object(tag);
                writer.object_value_start();
                writer.value_string(value);
                writer.object_value_end();
                writer.object_end();
                break;
        }
    }

    void OnSequenceStart(Mark const&, string const& tag, anchor_t, EmitterStyle::value /*style*/) override {
        std::cerr << "Got tag {} with a sequence\n"_format(tag);
        new_object(tag);
        writer.object_value_start();
        writer.list_start();
        push_state(State::List);
    }
    void OnSequenceEnd() override {
        writer.list_end();
        writer.object_value_end();
        writer.object_end();
        pop_state(State::List);
        update_map_state();
    }

    void OnMapStart(Mark const&, string const& tag, anchor_t, EmitterStyle::value /*style*/) override {
        std::cerr << "Got tag {} with a map\n"_format(tag);
        new_object(tag);
        push_state(State::Map);
    }
    void OnMapEnd() override {
        writer.object_end();
        pop_state(State::Map);
        update_map_state();
    }

    void OnAnchor(Mark const&, string const& anchor_name) override {
        std::cerr << "Got anchor {}\n"_format(anchor_name);
        anchor = anchor_name;
        anchors.push_back(anchor);
    }

public:
    W const& get_node_reader() const {
        return writer;
    }

public:
    void state_mismatch_error(State s) const override {
        throw DeserializationError("Got state {} while expecting {}"_format((int)state(), (int)s));
    }

protected:
    void new_object(string const& tag) {
        auto id = anchor.empty() ? random_id() : s_to_id(anchor);
        writer.object_start(id);
        if (tag.size() > 1 && tag[0] == '!') {
            auto type = tag.substr(1);
            writer.type(type);
        } else {
            writer.auto_type();
        }
        anchor.clear();
    }
    void update_map_state() {
        if (state() == State::Value)
            set_state(State::Map);
    }

private:
    W writer;
    string anchor;
    vector<string> anchors;
};

} // namespace rainynite::core::serialize

namespace rainynite::core::filters {

struct DummyReader {};

class YamlReader : FILTER_READ(YamlReader) {
    FILTER_NAME("yaml")

public:
    shared_ptr<AbstractDocument> read_document(std::istream& input) const override {
        serialize::YamlCppWrapper<serialize::NodeDeserializer<DummyReader>> node_reader;
        YAML::Parser parser(input);
        parser.HandleNextDocument(node_reader);
        return dynamic_pointer_cast<AbstractDocument>(node_reader.get_node_reader().get_root());
    }
};

} // namespace rainynite::core::filters
