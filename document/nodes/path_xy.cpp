/*
 *  path_xy.cpp - get path's y at given x
 *  Copyright (C) 2016-2017 caryoscelus
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

#include <algorithm>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <core/all_types.h>
#include <core/node_info.h>
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/time/format.h>
#include <core/context.h>

#include <geom_helpers/knots.h>

namespace core {
namespace nodes {

class PathXY : public Node<double> {
public:
    PathXY() {
        init<Geom::BezierKnots>(path, {});
        init<double>(x, 0);
    }
public:
    double get(std::shared_ptr<Context> ctx) const override {
        auto path = Geom::knots_to_path(get_path()->get(ctx));
        auto x = std::max(
            path.initialPoint().x(),
            std::min(
                path.finalPoint().x(),
                get_x()->get(ctx)
            )
        );
        auto path_roots = path.roots(x, Geom::X);
        // since x is in range between initial and final point, we should always get a root
        if (path_roots.size() < 1) {
            throw std::runtime_error("TimeCurve: no roots found at time: {}"_format(ctx->get_time()));
            return 0;
        }
        // enable this when upstream issue is resolved
        // https://github.com/inkscape/lib2geom/issues/14
        // 	if (path_roots.size() > 1)
        // 		throw std::runtime_error("TimeCurve: too much roots found at time: {}"_format(t));
        auto path_t = path_roots[0];
        return path.valueAt(path_t, Geom::Y);
    }

private:
    NODE_PROPERTY(path, Geom::BezierKnots);
    NODE_PROPERTY(x, double);
};

REGISTER_NODE(PathXY);

} // namespace nodes
} // namespace core
