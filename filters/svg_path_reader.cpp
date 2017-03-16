/*
 *  svg_path_reader.cpp - primitive document reader for testing
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

#include <core/filters/svg_path_reader.h>
#include <core/document.h>

#include <geom_helpers/knots.h>

namespace core {

namespace filters {

std::unique_ptr<Document> SvgPathReader::read_document(std::istream& input) {
    std::vector<Geom::BezierKnots> paths;
    while (input.good()) {
        std::string svg_path;
        std::getline(input, svg_path, ';');
        paths.push_back(Geom::svg_to_knots(svg_path));
    }
    return std::make_unique<Document>(paths);
}

} // namespace filters

} // namespace core
