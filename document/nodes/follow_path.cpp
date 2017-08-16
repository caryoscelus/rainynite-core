/*
 *  follow_path.cpp - follow path node
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/context.h>

#include <geom_helpers/knots.h>

namespace core {
namespace nodes {

class FollowPath : public Node<Geom::Point> {
public:
    FollowPath() {
        init<Geom::BezierKnots>(path, {});
    }
public:
    Geom::Point get(std::shared_ptr<Context> ctx) const override {
        try {
            auto path = get_path()->get(ctx);
            auto t = ctx->get_time().get_seconds();
            return Geom::knots_to_path(path).pointAt(t);
        } catch (...) {
            return {};
        }
    }

private:
    NODE_PROPERTY(path, Geom::BezierKnots);
};

REGISTER_NODE(FollowPath);

} // namespace nodes
} // namespace core
