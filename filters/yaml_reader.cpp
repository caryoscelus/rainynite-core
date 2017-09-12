/*
 *  yaml_reader.cpp - YAML document reader
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

#include <iostream>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/random_generator.hpp>

#include <fmt/format.h>

#include <yaml-cpp/eventhandler.h>
#include <yaml-cpp/parser.h>

#include <core/document.h>
#include <core/node_info.h>
#include <core/filters/yaml_reader.h>
#include <core/serialize/node_reader.h>

using namespace fmt::literals;

namespace rainynite::core::serialize {

static boost::uuids::string_generator s_to_id;
static boost::uuids::random_generator random_id;

template <class W>
class YamlCppWrapper : public YAML::EventHandler {
    using Mark = YAML::Mark;
    using anchor_t = YAML::anchor_t;
    using EmitterStyle = YAML::EmitterStyle;
public:
    enum class Status {
        Map,
        Value,
        List
    };
public:
    void OnDocumentStart(Mark const&) override {
    }
    void OnDocumentEnd() override {
    }

    void OnNull(Mark const&, anchor_t) override {
        throw DeserializationError("null is not supported (yet)");
    }
    void OnAlias(Mark const&, anchor_t) override {
        std::cerr << "Got alias\n";
    }
    void OnScalar(Mark const&, string const& tag, anchor_t, string const& value) override {
        std::cerr << "Got tag {} with value {}\n"_format(tag, value);
        switch (status()) {
            case Status::Map: {
                if (tag != "?")
                    throw DeserializationError("Got key {} with a tag {}"_format(value, tag));
                if (!anchor.empty())
                    throw DeserializationError("Got key {} with an anchor {}"_format(value, anchor));
                writer.key(value);
                set_status(Status::Value);
            } break;
            case Status::Value:
                set_status(Status::Map);
                [[fallthrough]];
            case Status::List:
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
        push_status(Status::List);
    }
    void OnSequenceEnd() override {
        writer.list_end();
        writer.object_value_end();
        writer.object_end();
        pop_status(Status::List);
        if (status() == Status::Value)
            set_status(Status::Map);
    }

    void OnMapStart(Mark const&, string const& tag, anchor_t, EmitterStyle::value /*style*/) override {
        std::cerr << "Got tag {} with a map\n"_format(tag);
        new_object(tag);
        push_status(Status::Map);
    }
    void OnMapEnd() override {
        writer.object_end();
        pop_status(Status::Map);
        if (status() == Status::Value)
            set_status(Status::Map);
    }

    void OnAnchor(Mark const&, string const& anchor_name) override {
        std::cerr << "Got anchor {}\n"_format(anchor_name);
        anchor = anchor_name;
    }

public:
    W const& get_node_reader() const {
        return writer;
    }

protected:
    void new_object(string const& tag) {
        if (tag[0] != '!')
            throw DeserializationError("Got unknown tag {}"_format(tag));
        auto type = tag.substr(1);
        auto id = anchor.empty() ? random_id() : s_to_id(anchor);
        writer.object_start(id);
        std::replace(type.begin(), type.end(), '(', '<');
        std::replace(type.begin(), type.end(), ')', '>');
        writer.type(type);
        anchor.clear();
    }
    Status status() {
        return status_stack.back();
    }
    void push_status(Status s) {
        status_stack.push_back(s);
    }
    void pop_status(Status s) {
        if (status() != s)
            throw DeserializationError("Got status {} while expecting {}"_format((int)s, (int)status()));
        status_stack.pop_back();
    }
    void set_status(Status s) {
        status_stack.back() = s;
    }

private:
    W writer;
    string anchor;
    vector<Status> status_stack;
};

} // namespace rainynite::core::serialize

namespace rainynite::core::filters {

struct DummyReader {};

shared_ptr<Document> YamlReader::read_document(std::istream& input) {
    serialize::YamlCppWrapper<serialize::NodeDeserializer<DummyReader>> node_reader;
    YAML::Parser parser(input);
    parser.HandleNextDocument(node_reader);
    return dynamic_pointer_cast<Document>(node_reader.get_node_reader().get_root());
}

} // namespace rainynite::core::filters
