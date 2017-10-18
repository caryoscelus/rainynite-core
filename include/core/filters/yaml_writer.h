/*  filters/yaml_writer.h - yaml serializer
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

#ifndef CORE_FILTERS_YAML_WRITER_H_915BC576_E30C_5035_AFD6_BB344DA75AD2
#define CORE_FILTERS_YAML_WRITER_H_915BC576_E30C_5035_AFD6_BB344DA75AD2

#include "writer.h"

namespace rainynite::core::filters {

class YamlWriter : public DocumentWriter {
public:
    void write_document(std::ostream& output, shared_ptr<Document> document) override;
};

} // namespace rainynite::core::filters

#endif
