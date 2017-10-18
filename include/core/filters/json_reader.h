/*  filters/json_reader.h - simple JSON document reader
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

#ifndef CORE_SERIALIZE_JSON_READER_H_5D691790_F69D_565E_887B_9666CEADCB86
#define CORE_SERIALIZE_JSON_READER_H_5D691790_F69D_565E_887B_9666CEADCB86

#include "reader.h"

namespace rainynite::core::filters {

class JsonReader : public DocumentReader {
public:
    shared_ptr<Document> read_document(std::istream& input) override;
};

} // namespace rainynite::core::filters

#endif
