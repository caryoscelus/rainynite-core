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
#include <core/node_info.h>
#include <core/nodes/animated.h>
#include <core/color.h>
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

    auto animated_node = make_node_with_name<AbstractValue>("Animated<BezierPath>");
    auto animated = dynamic_cast<nodes::AbstractAnimated*>(animated_node.get());
    for (int i = 0; i < (int)paths.size()-1; ++i) {
        auto first = make_value<Geom::BezierKnots>(paths[i]);
        auto second = make_value<Geom::BezierKnots>(paths[i+1]);
        auto morph_node = make_node_with_name<AbstractValue>("BezierMorph");
        auto morph = dynamic_cast<AbstractNode*>(morph_node.get());
        morph->set_property("a", first);
        morph->set_property("b", second);
        animated->add_child({Time(i), Time(i+1)}, morph_node);
    }

    auto path_shape_node = make_node_with_name<AbstractValue>("PathShape");
    auto path_shape = dynamic_cast<AbstractNode*>(path_shape_node.get());
    path_shape->set_property("path", animated_node);
    path_shape->set_property("fill_color", make_value<colors::Color>(0xff, 0, 0, 0xff));

    auto composite_node = make_node_with_name<BaseValue<Renderable>>("Composite");
    auto composite = dynamic_cast<AbstractNode*>(composite_node.get());
    auto layers = dynamic_cast<AbstractListLinked*>(composite->get_property("layers").get());
    layers->push_back(path_shape_node);

    return std::make_shared<Document>(composite_node);
}

} // namespace filters

} // namespace core
