/*  bones.cpp - bone related nodes
 *  Copyright (C) 2018 caryoscelus
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>

#include <geom_helpers/bones.h>

namespace rainynite::core::nodes {

class BoneP :
    public NewNode<
        BoneP,
        Geom::bones::Bone,
        types::Only<Geom::Point>,
        types::Only<Geom::Point>
    >
{
    DOC_STRING("Composite Bone")

    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(Geom::Point{}, Geom::Point{})

    PROPERTY(a)
    PROPERTY(b)

protected:
    Geom::bones::Bone get(shared_ptr<Context> ctx) const override {
        using Geom::Point;
        auto a = a_value<Point>(ctx);
        return { a, b_value<Point>(ctx)-a };
    }
};

REGISTER_NODE(BoneP);

class BoneTransform :
    public NewNode<
        BoneTransform,
        Geom::Affine,
        types::Only<Geom::bones::Bone>,
        types::Only<Geom::bones::Bone>
    >
{
    DOC_STRING(
        "Bone diff transform"
    )

    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(Geom::bones::Bone{}, Geom::bones::Bone{});

    PROPERTY(a)
    PROPERTY(b)

protected:
    Geom::Affine get(shared_ptr<Context> ctx) const override {
        using namespace Geom::bones;
        return bone_diff(a_value<Bone>(ctx), b_value<Bone>(ctx));
    }

};

REGISTER_NODE(BoneTransform);

} // namespace rainynite::core::nodes
