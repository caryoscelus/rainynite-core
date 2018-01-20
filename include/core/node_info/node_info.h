/*  node_info.h - runtime node information
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

#ifndef CORE_NODE_INFO_H_E59BD832_E858_5D6E_83EC_79AF65B01A25
#define CORE_NODE_INFO_H_E59BD832_E858_5D6E_83EC_79AF65B01A25

#include <core/std/set.h>
#include <core/util/class_init.h>
#include <core/util/type_info.h>
#include <core/node/abstract_value.h>

namespace rainynite::core {

/**
 * Runtime node information interface.
 */
class NodeInfo {
public:
    string operator()() const {
        return name();
    }
    virtual string name() const = 0;
    virtual AbstractReference new_empty() const = 0;
    virtual AbstractReference clone(AbstractValue const& source) const = 0;
    virtual Type type() const = 0;
};

/**
 * Get node info for given node class
 */
inline NodeInfo const& get_node_info(std::type_index node_type) {
    return class_init::type_meta<NodeInfo>(node_type);
}

/// Get node name for given type if it's a type of registered node
string node_type_name(std::type_index node_type);

/// Get node name
inline string node_name(AbstractValue const& node) {
    return node_type_name(typeid(node));
}

inline NodeInfo const& get_node_type(string const& name) {
    return get_node_info(class_init::find_type(name));
}

/**
 * Make node with given name, optionally using source & context to initialize it.
 *
 * TODO: rename to make_node_with_name & that to make_node_with_name_as
 */
AbstractReference make_node_with_name_base(string const& name, AbstractReference source=nullptr, shared_ptr<Context> context=nullptr);

/**
 * Same as make_node_with_name_base, but optionally cast to desired type.
 *
 * TODO: throw instead of returning nullptr?
 */
template <typename T=AbstractValue, typename... Args>
shared_ptr<T> make_node_with_name(Args&&... args) {
    return dynamic_pointer_cast<T>(make_node_with_name_base(std::forward<Args>(args)...));
}

inline map<Type, set<NodeInfo const*>>& node_types() {
    static map<Type, set<NodeInfo const*>> instance;
    return instance;
}

set<NodeInfo const*> all_node_infos();

template <class I>
struct RegisterNodeByType : class_init::Initialized<RegisterNodeByType<I>> {
    static void init() {
        auto info = new I();
        node_types()[info->type()].insert(info);
    }
};

} // namespace rainynite::core

#endif
