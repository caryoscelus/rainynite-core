/*  document.cpp - document
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
#include <core/document.h>
#include <core/context.h>
#include <core/node_tree.h>
#include <core/action_stack.h>

namespace rainynite::core {

REGISTER_NODE(Document);

Document::Document(shared_ptr<BaseValue<Renderable>> root_) :
    default_context(nullptr),
    action_stack(make_shared<ActionStack>())
{
    init<Renderable>(root, {});
    init<Geom::Point>(size, {320, 240});
    init<TimePeriod>(main_time_period, {Time(0, 12), Time(5, 12)});
    // TODO: use constraints
    init_property(soundtrack, Type(typeid(Audio)), make_node_with_name("AudioFromFile"));
    if (root_)
        set_root(root_);
    else
        set_root(make_node_with_name<BaseValue<Renderable>>("Composite"));
    new_id();
}

Document::~Document() {
}

shared_ptr<NodeTree> Document::get_tree() {
    if (tree == nullptr)
        tree = make_shared<NodeTree>(static_pointer_cast<Document>(shared_from_this()), action_stack);
    return tree;
}

DocumentType Document::get(shared_ptr<Context> /*context*/) const {
    return {};
}

shared_ptr<Context> Document::get_default_context() {
    if (!default_context) {
        default_context = make_shared<Context>(static_pointer_cast<Document>(shared_from_this()));
    }
    return default_context;
}

} // namespace rainynite::core
