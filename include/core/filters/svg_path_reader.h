/*
 *  svg_path_reader.h - primitive document reader for testing
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

#ifndef __CORE__FILTERS__SVG_PATH_READER_H__687A828C
#define __CORE__FILTERS__SVG_PATH_READER_H__687A828C

#include "../reader.h"

namespace core {

namespace filters {

class SvgPathReader : public DocumentReader {
public:
    virtual std::unique_ptr<Document> read_document(std::istream& input) override;
};

} // namespace filters

} // namespace core

#endif
