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

#include <core/document.h>
#include <core/nodes/morph.h>
#include <core/filters/svg_path_reader.h>

#include <geom_helpers/knots.h>

namespace core {

namespace filters {

Geom::BezierKnots parse_string(std::string const& line) {
    std::istringstream stream(line);
    std::string svg_path, keys;
    std::getline(stream, svg_path, '@');
    std::getline(stream, keys, '@');
    auto path = Geom::svg_to_knots(svg_path);
    unsigned index = 0;
    std::istringstream keystream(keys);
    while (keystream.good()) {
        std::string key;
        std::getline(keystream, key, ',');
        path.knots[index].uid = key;
        ++index;
    }
    return path;
}

std::shared_ptr<Document> SvgPathReader::read_document(std::istream& input) {
    std::vector<Geom::BezierKnots> paths;
    while (input.good()) {
        std::string path;
        std::getline(input, path, ';');
        try {
            paths.push_back(parse_string(path));
        } catch (...) {
        }
    }
    if (paths.size() != 2)
        return nullptr;
    auto morph = std::make_shared<nodes::BezierMorph>();
    morph->set_a(make_value<Geom::BezierKnots>(paths[0]));
    morph->set_b(make_value<Geom::BezierKnots>(paths[1]));
    return std::make_shared<Document>(morph);
}

} // namespace filters

} // namespace core
