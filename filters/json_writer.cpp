/*
 *  json_writer.cpp - simple json serializer
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

#include <core/document.h>
#include <core/serialize/node_writer.h>
#include <core/serialize/json.h>
#include <core/filters/json_writer.h>

namespace rainynite::core {
namespace filters {

void JsonWriter::write_document(std::ostream& output, shared_ptr<Document> document) {
    auto writer = serialize::DumbJsonWriter<serialize::NodeWriter, AbstractReference>(output);
    writer.object(std::dynamic_pointer_cast<AbstractValue>(document));
}

} // namespace filters
} // namespace rainynite::core
