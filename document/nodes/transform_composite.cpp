/*  transform_composite.cpp - combine transforms from list
 *  Copyright (C) 2017-2018 caryoscelus
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

#include <core/node/new_node.h>
#include <core/node/list.h>
#include <core/node_info.h>

#include <2geom/affine.h>

namespace rainynite::core::nodes {

class TransformComposite :
    public NewNode<
        TransformComposite,
        Geom::Affine,
        types::Only<vector<Geom::Affine>>
    >
{
    DOC_STRING(
        "Combine transforms from list."
    )

    NODE_PROPERTIES("transforms")
    static vector<AbstractReference> const& default_values() {
        static vector<AbstractReference> instance {
            make_node<ListValue<Geom::Affine>>()
        };
        return instance;
    }
    PROPERTY(transforms)

protected:
    Geom::Affine get(shared_ptr<Context> ctx) const override {
        Geom::Affine result;
        for (auto const& t : transforms_value<vector<Geom::Affine>>(ctx)) {
            result *= t;
        }
        return result;
    }
};

REGISTER_NODE(TransformComposite);

} // namespace rainynite::core::nodes
