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
#include "abstract_list.h"
#include "doc_string.h"

namespace rainynite::core {

/**
 * Abstract node: entity with links to AbstractValues
 */
class AbstractNode : public AbstractListLinked, public DocString {
public:
    virtual ~AbstractNode();
public:
    AbstractReference get_property(string const& name) const;
    template <typename T>
    shared_ptr<BaseValue<T>> get_property_as(string const& name) const {
        return dynamic_pointer_cast<BaseValue<T>>(get_property(name));
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
    void set_property(string const& name, AbstractReference ref);
    bool remove_property(string const& name);
    size_t init_property(string const& name, TypeConstraint type, AbstractReference value);
    map<string, AbstractReference> get_link_map() const;
    string get_name_at(size_t id) {
        return names_list[id];
    }
public:
    vector<AbstractReference> get_links() const override {
        return numbered_storage;
    }
    AbstractReference get_link(size_t i) const override {
        return get_by_id(i);
    }
    TypeConstraint get_link_type(size_t i) const override {
        return types[i];
    }
    void set_link(size_t i, AbstractReference value) override;
    size_t link_count() const override {
        return numbered_storage.size();
    }
protected:
    /**
     * This function should be called when node has changed.
     *
     * In practice, it exists solely due to class hierarchy and lack of
     * AbstractValue inheritance in AbstractNode.
     */
    virtual void node_changed() = 0;
private:
    AbstractReference const& get_by_id(size_t index) const {
        return numbered_storage[index];
    }
    AbstractReference& get_by_id(size_t index) {
        return numbered_storage[index];
    }
private:
    map<string, size_t> named_storage;
    vector<string> names_list;
    vector<AbstractReference> numbered_storage;
    vector<boost::signals2::connection> signal_connections;
    vector<TypeConstraint> types;
};

} // namespace rainynite::core

#endif
