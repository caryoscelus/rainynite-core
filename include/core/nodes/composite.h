/*
 *  composite.h - Simple composite render node
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

#ifndef __CORE__NODES__COMPOSITE_H__266BAD3C
#define __CORE__NODES__COMPOSITE_H__266BAD3C

#include <core/node.h>
#include <core/node_info.h>
#include <core/renderable.h>

namespace core {
namespace nodes {

class Composite : public Node<Renderable> {
public:
    Composite() {
        init_list<Renderable>(layers);
        get_layers()->new_id();
    }
public:
    virtual Renderable get(Time time) const override {
        return Renderable();
    }
private:
    NODE_LIST_PROPERTY(layers, Renderable);
};

REGISTER_NODE(Composite);

} // namespace nodes
} // namespace core

#endif
