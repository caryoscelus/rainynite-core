/*  node_info.cpp - runtime node information
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

#include <core/node_info/node_info.h>
#include <core/node_info/default_node.h>
#include <core/node_info/copy.h>

namespace rainynite::core {

string node_type_name(std::type_index type) {
    try {
        return class_init::type_info<NodeInfo,string>(type);
    } catch (class_init::RuntimeTypeError const& ex) {
        return "unknown";
    }
}

AbstractReference make_node_with_name(string const& name, AbstractReference source, shared_ptr<Context> context) {
    auto node = get_node_type(name).new_empty();
    node->new_id(); // TODO: don't do that here?
    if (source && context) {
        auto value = source->get_any(context);
        if (node->can_set_source(source))
            node->set_source(source);
        else if (source && node->can_set_any(value))
            node->set_any(value);
    }
    return node;
}

AbstractReference shallow_copy(AbstractValue const& source) {
    auto node = get_node_info(typeid(source)).clone(source);
    node->new_id();
    return node;
}

AbstractReference deep_copy(AbstractValue const& source) {
    auto value = shallow_copy(source);
    if (auto node = list_cast(value)) {
        for (size_t i = 0; i < node->link_count(); ++i) {
            node->set_link(i, deep_copy(*node->get_link(i)));
        }
    }
    return value;
}

set<NodeInfo const*> all_node_infos() {
    set<NodeInfo const*> result;
    for (auto const& e : node_types()) {
        result.insert(e.second.begin(), e.second.end());
    }
    return result;
}

AbstractReference make_default_node(Type type) {
    auto node = get_default_node_info(type).new_empty();
    node->new_id(); // TODO
    return node;
}

} // namespace rainynite::core
