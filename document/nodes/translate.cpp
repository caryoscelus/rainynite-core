/*  translate.cpp - Translate transformation node
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

#include <core/node/node.h>
#include <core/node/property.h>
#include <core/node_info.h>

#include <2geom/transforms.h>

namespace rainynite::core::nodes {

class Translate : public Node<Geom::Affine> {
    DOC_STRING(
        "Construct Translate transformation."
    )
public:
    Translate() {
        init<Geom::Affine>(source, {});
        init<Geom::Point>(offset, {});
    }

    Geom::Affine get(shared_ptr<Context> ctx) const override {
        return
            get_source()->value(ctx) *
            Geom::Translate(get_offset()->value(ctx));
    }

private:
    NODE_PROPERTY(source, Geom::Affine);
    NODE_PROPERTY(offset, Geom::Point);
};

REGISTER_NODE(Translate);

} // namespace rainynite::core::nodes
