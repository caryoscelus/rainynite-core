/*
 *  json_reader.cpp - simple JSON document reader
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

#include <rapidjson/istreamwrapper.h>

#include <core/document.h>
#include <core/node_info.h>
#include <core/filters/json_reader.h>
#include <core/serialize/rapidjson.h>
#include <core/serialize/node_reader.h>

namespace rainynite::core {

namespace filters {

struct DummyReader {};

shared_ptr<Document> JsonReader::read_document(std::istream& input) {
    serialize::RJSONWrapper<serialize::NodeDeserializer<DummyReader>> node_reader;
    rapidjson::Reader json_reader;
    rapidjson::IStreamWrapper stream(input);
    json_reader.Parse(stream, node_reader);
    return std::dynamic_pointer_cast<Document>(node_reader.get_node_reader().get_root());
}

} // namespace filters

} // namespace rainynite::core
