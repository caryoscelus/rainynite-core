/*
 *  svg_path_writer.cpp - primitive document writer for testing
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

#include <core/filters/svg_path_writer.h>
#include <core/document.h>

#include <geom_helpers/knots.h>

namespace core {

namespace filters {

void SvgWriter::write_document(std::ostream& output, Document const& document) {
    throw "Not implemented";
}

} // namespace filters

} // namespace core
