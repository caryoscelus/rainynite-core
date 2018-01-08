/*  affine.cpp - affine transformations
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

#include <boost/math/constants/constants.hpp>

#include <core/node/new_node.h>
#include <core/node_info/macros.h>

#include <2geom/transforms.h>

namespace rainynite::core::nodes {

class Translate :
    public NewNode<
        Translate,
        Geom::Affine,
        types::Only<Geom::Point>
    >
{
    DOC_STRING(
        "Construct Translate transformation."
    )

    NODE_PROPERTIES("offset")
    DEFAULT_VALUES(Geom::Point{})
    PROPERTY(offset)

protected:
    Geom::Affine get(shared_ptr<Context> ctx) const override {
        return Geom::Translate(offset_value<Geom::Point>(ctx));
    }
};

REGISTER_NODE(Translate);


class Rotate :
    public NewNode<
        Rotate,
        Geom::Affine,
        types::Only<Geom::Point>,
        types::Only<double>
    >
{
    DOC_STRING(
        "Rotate transformation."
    )

    NODE_PROPERTIES("origin", "angle")
    DEFAULT_VALUES(Geom::Point{}, 0.0)
    PROPERTY(origin)
    PROPERTY(angle)

protected:
    Geom::Affine get(shared_ptr<Context> ctx) const override {
        using boost::math::double_constants::pi;
        auto offset = origin_value<Geom::Point>(ctx);
        return
            Geom::Translate(-offset) *
            Geom::Rotate(angle_value<double>(ctx) * pi * 2) *
            Geom::Translate(offset);
    }
};

REGISTER_NODE(Rotate);


class Scale :
    public NewNode<
        Scale,
        Geom::Affine,
        types::Only<Geom::Point>,
        types::Only<Geom::Point>
    >
{
    DOC_STRING(
        "Scale transformation."
    )

    NODE_PROPERTIES("origin", "scale")
    DEFAULT_VALUES(Geom::Point{}, Geom::Point{1, 1})
    PROPERTY(origin)
    PROPERTY(scale)

protected:
    Geom::Affine get(shared_ptr<Context> ctx) const override {
        auto offset = origin_value<Geom::Point>(ctx);
        return
            Geom::Translate(-offset) *
            Geom::Scale(scale_value<Geom::Point>(ctx)) *
            Geom::Translate(offset);
    }
};

REGISTER_NODE(Scale);

} // namespace rainynite::core::nodes
