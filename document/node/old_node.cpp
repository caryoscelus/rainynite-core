/*  old_node.cpp - old Node
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

#include <core/node/node.h>

namespace rainynite::core {

BaseOldNode::~BaseOldNode() {
    for (auto const& connection : signal_connections) {
        connection.disconnect();
    }
}

AbstractReference BaseOldNode::get_property(string const& name) const {
    auto result = named_storage.find(name);
    if (result == named_storage.end())
        throw NodeAccessError("Unknown property "+name);
    return get_by_id(result->second);
}

void BaseOldNode::set_property(string const& name, AbstractReference ref) {
    if (named_storage.count(name) == 0) {
        if (name[0] == '_') {
            // accept as custom attribute
            init_property(name, {}, ref);
        } else
            throw NodeAccessError("No such property: "+name);
    }
    set_link(named_storage[name], ref);
}

bool BaseOldNode::remove_property(string const& name) {
    if (name[0] != '_')
        return false; // can only remove custom props
    auto iter = named_storage.find(name);
    if (iter == std::end(named_storage))
        return false;
    auto i = iter->second;
    // NOTE: this might be not thread-safe
    named_storage.erase(iter);
    names_list.erase(names_list.begin()+i);
    numbered_storage.erase(numbered_storage.begin()+i);
    // reassign links
    for (auto& e : named_storage) {
        if (e.second > i)
            --e.second;
    }
    return true;
}

size_t BaseOldNode::init_property(string const& name, TypeConstraint type, AbstractReference value) {
    size_t id = link_count();
    numbered_storage.push_back(value);
    names_list.push_back(name);
    named_storage[name] = id;
    types.push_back(type);
    boost::signals2::connection connection;
    if (value)
        connection = value->subscribe([this]() {
            node_changed();
        });
    signal_connections.push_back(connection);
    return id;
}

map<string, AbstractReference> BaseOldNode::get_link_map() const {
    map<string, AbstractReference> result;
    // TODO: use generic conversion function
    for (auto const& e : named_storage) {
        result.emplace(e.first, get_by_id(e.second));
    }
    return result;
}

void BaseOldNode::set_link(size_t i, AbstractReference value) {
    if (!get_link_type(i).accept(value->get_type()))
        throw NodeAccessError("Node property type mis-match");
    signal_connections[i].disconnect();
    get_by_id(i) = value;
    signal_connections[i] = value->subscribe([this]() {
        node_changed();
    });
    node_changed();
}

} // namespace rainynite::core
