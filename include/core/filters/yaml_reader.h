/*  filters/yaml_reader.h - YAML document reader
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

#ifndef CORE_SERIALIZE_YAML_READER_H_3AEA9FF2_1EC1_5130_81AE_BAD16615841D
#define CORE_SERIALIZE_YAML_READER_H_3AEA9FF2_1EC1_5130_81AE_BAD16615841D

#include "reader.h"

namespace rainynite::core::filters {

class YamlReader : public DocumentReader {
public:
    shared_ptr<AbstractDocument> read_document(std::istream& input) override;
};

} // namespace rainynite::core::filters

#endif
