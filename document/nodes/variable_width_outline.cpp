/*  variable_width_outline.cpp - construct variable width outline node
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
#include <core/node/list.h>
#include <core/bezier_outline.h>

namespace rainynite::core::nodes {

class SvgVariableWidthOutline :
    public NewNode<
        SvgVariableWidthOutline,
        BezierOutlinePath,
        types::Only<Geom::BezierKnots>,
        types::Only<vector<double>>,
        types::Only<vector<double>>
    >
{
    DOC_STRING(
        "Make svg-based variable width outline\n"
        "\n"
        "Note that this might not work with non-svg renderers.."
    )

    NODE_PROPERTIES("source", "indexes", "widths")
    COMPLEX_DEFAULT_VALUES(make_value<Geom::BezierKnots>(), make_node<ListValue<double>>(), make_node<ListValue<double>>())
    PROPERTY(source)
    PROPERTY(indexes)
    PROPERTY(widths)

protected:
    BezierOutlinePath get(shared_ptr<Context> ctx) const override {
        vector<pair<double,double>> indexed_widths;
        auto indexes = indexes_value<vector<double>>(ctx);
        auto widths = widths_value<vector<double>>(ctx);
        for (
            auto index_i = indexes.begin(), width_i = widths.begin();
            index_i != indexes.end() && width_i != widths.end();
            ++index_i, ++width_i
        ) {
            indexed_widths.emplace_back(*index_i, *width_i);
        }
        return {
            source_value<Geom::BezierKnots>(ctx),
            indexed_widths
        };
    }
};

REGISTER_NODE(SvgVariableWidthOutline);

} // namespace rainynite::core::nodes
