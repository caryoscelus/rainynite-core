/*
 *  translate.cpp - Translate transformation render node
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

#include <2geom/point.h>

namespace core {
namespace nodes {

class Translate : public RenderableNode {
public:
    Translate() {
        init<Renderable>(source, {});
        init<Geom::Point>(offset, {});
    }
    virtual std::string get_source_name() const override {
        return "source";
    }

private:
    NODE_PROPERTY(source, Renderable);
    NODE_PROPERTY(offset, Geom::Point);
};

REGISTER_NODE(Translate);

} // namespace nodes
} // namespace core
