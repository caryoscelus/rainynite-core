/*  transform_path.cpp - node that applies transformation to bezier path points
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

#include <core/node_info/macros.h>
#include <core/node/new_node.h>
#include <core/node/list.h>

#include <geom_helpers/knots.h>

#include <2geom/affine.h>

namespace rainynite::core::nodes {

class TransformPath :
    public NewNode<
        TransformPath,
        Geom::BezierKnots,
        types::Only<Geom::BezierKnots>,
        types::Only<Geom::Affine>,
        types::Only<vector<double>>
    >
{
    DOC_STRING(
        "Apply affine transformation to bezier path elements.\n"
        "\n"
        "\n"
    )

    NODE_PROPERTIES("source", "transform", "weights")
//     DEFAULT_VALUES(Geom::BezierKnots{}, Geom::Affine{}, vector<double>{});
    static vector<AbstractReference> const& default_values() {
        static vector<AbstractReference> instance {
            make_value<Geom::BezierKnots>(),
            make_value<Geom::Affine>(),
            make_node<ListValue<double>>()
        };
        return instance;
    }

    PROPERTY(source)
    PROPERTY(transform)
    PROPERTY(weights)

protected:
    Geom::BezierKnots get(shared_ptr<Context> ctx) const override {
        using namespace Geom;
        auto knots = source_value<BezierKnots>(ctx);
        auto transform = transform_value<Affine>(ctx);
        auto apply_transform = [transform](auto const& point, double weight) {
            auto transformed = point * transform;
            return transformed * weight + point * (1.0-weight);
        };
        auto weights = weights_value<vector<double>>(ctx);
        auto knot = knots.knots.begin();
        auto weight = weights.begin();
        for (; knot != knots.knots.end() && weight != weights.end(); ++knot, ++weight) {
            auto pos = knot->pos;
            knot->pos = apply_transform(pos, *weight);
            knot->tg1 = apply_transform(knot->tg1+pos, *weight) - knot->pos;
            knot->tg2 = apply_transform(knot->tg2+pos, *weight) - knot->pos;
        }
        return knots;
    }

};

REGISTER_NODE(TransformPath);

} // namespace rainynite::core::nodes
