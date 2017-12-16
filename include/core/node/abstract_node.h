/*  abstract_node.h - abstract Node
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

#ifndef CORE_NODE_ABSTRACT_NODE_H_EE148DA4_6D84_5EDA_A5B0_C192A55C5B09
#define CORE_NODE_ABSTRACT_NODE_H_EE148DA4_6D84_5EDA_A5B0_C192A55C5B09

#include <boost/signals2/signal.hpp>

#include <core/std/map.h>
#include <core/util/doc_string.h>
#include "abstract_list.h"

namespace rainynite::core {

class AbstractNode {
public:
    virtual AbstractReference get_property(string const& name) const = 0;
    virtual void set_property(string const& name, AbstractReference ref) = 0;
    virtual bool remove_property(string const& /*name*/) {
        return false;
    }
    virtual string get_name_at(size_t id) const = 0;
    virtual size_t get_name_id(string const& name) const = 0;
    virtual map<string, AbstractReference> get_link_map() const = 0;

protected:
    /**
     * This function should be called when node has changed.
     *
     * In practice, it exists solely due to class hierarchy and lack of
     * AbstractValue inheritance in AbstractNode.
     */
    virtual void node_changed() = 0;

public:
    template <typename T>
    shared_ptr<BaseValue<T>> get_property_as(string const& name) const {
        return base_value_cast<T>(get_property(name));
    }
    template <typename T>
    optional<T> get_property_value(string const& name, shared_ptr<Context> context) const {
        try {
            if (auto node = get_property_as<T>(name))
                return node->value(context);
            return {};
        } catch (NodeAccessError const&) {
            return {};
        } catch (...) {
            return {};
        }
    }
};

} // namespace rainynite::core

#endif
