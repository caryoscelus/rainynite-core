/*
 *  json_writer.h - simple json serializer
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

#ifndef __CORE__FILTERS__JSON_WRITER_H__CC8DB984
#define __CORE__FILTERS__JSON_WRITER_H__CC8DB984

#include "../writer.h"

namespace rainynite::core {
namespace filters {

class JsonWriter : public DocumentWriter {
public:
    void write_document(std::ostream& output, std::shared_ptr<Document> document) override;
};

} // namespace filters
} // namespace rainynite::core

#endif
