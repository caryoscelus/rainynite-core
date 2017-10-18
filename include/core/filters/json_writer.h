/*  filters/json_writer.h - simple json serializer
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

#ifndef CORE_SERIALIZE_JSON_WRITER_H_86F22847_207E_5BFB_9845_F2194F2ECCE7
#define CORE_SERIALIZE_JSON_WRITER_H_86F22847_207E_5BFB_9845_F2194F2ECCE7

#include "writer.h"

namespace rainynite::core::filters {

class JsonWriter : public DocumentWriter {
public:
    void write_document(std::ostream& output, shared_ptr<Document> document) override;
};

} // namespace rainynite::core::filters

#endif
