/*  new_node.h - add node for testing
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

#ifndef CORE_TESTS_NEW_NODE_H_7503A372_0E70_58C3_8595_93D47E5089CE
#define CORE_TESTS_NEW_NODE_H_7503A372_0E70_58C3_8595_93D47E5089CE

#include <core/node/new_node.h>

using namespace rainynite;
using namespace rainynite::core;

class Add :
    public NewNode<
        Add,
        double,
        types::Only<double>,
        types::Only<double>
    >
{
    NODE_PROPERTIES("a", "b")
    DEFAULT_VALUES(0.0, 0.0)
    PROPERTY(a)
    PROPERTY(b)

    DOC_STRING(
        "Simple Add node for testing..."
    )

    double get(shared_ptr<Context> ctx) const override {
        return a_value<double>(ctx) + b_value<double>(ctx);
    }
};

#endif
