/*
 *  image.cpp - Image import render node
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

#include <core/renderable.h>
#include <core/node_info.h>

namespace core {
namespace nodes {

class Image : public RenderableNode {
public:
    Image() {
        init<std::string>(file_path, "");
        init<double>(width, 0.0);
        init<double>(height, 0.0);
    }

private:
    NODE_PROPERTY(file_path, std::string);
    NODE_PROPERTY(width, double);
    NODE_PROPERTY(height, double);
};

REGISTER_NODE(Image);

} // namespace nodes
} // namespace core
