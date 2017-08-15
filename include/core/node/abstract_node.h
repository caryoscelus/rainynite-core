/*
 *  abstract_node.h - abstract Node
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

#ifndef __CORE__NODE__ABSTRACT_NODE_H__24ECBF82
#define __CORE__NODE__ABSTRACT_NODE_H__24ECBF82

#include <map>

#include <boost/signals2/signal.hpp>

#include "abstract_list.h"

namespace core {

class AbstractNode : public AbstractListLinked {
public:
    virtual ~AbstractNode();
public:
    AbstractReference get_property(std::string const& name) const;
    template <typename T>
    BaseReference<T> get_property_as(std::string const& name) const {
        return std::dynamic_pointer_cast<BaseValue<T>>(get_property(name));
    }
    template <typename T>
    boost::optional<T> get_property_value(std::string const& name, std::shared_ptr<Context> context) const {
        try {
            return get_property_as<T>(name)->get(context);
        } catch (NodeAccessError const&) {
            return boost::none;
        } catch (...) {
            return boost::none;
        }
    }
    void set_property(std::string const& name, AbstractReference ref);
    size_t init_property(std::string const& name, boost::optional<Type> type, AbstractReference value);
    std::map<std::string, AbstractReference> get_link_map() const;
    std::string get_name_at(size_t id) {
        return names_list[id];
    }
public:
    std::vector<NodeInContext> get_list_links(std::shared_ptr<Context> /*context*/) const override {
        throw NodeAccessError("get_list_links not implemented");
    }
    std::vector<AbstractReference> get_links() const override {
        return numbered_storage;
    }
    AbstractReference get_link(size_t i) const override {
        return get_by_id(i);
    }
    boost::optional<Type> get_link_type(size_t i) const override {
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
    std::map<std::string, size_t> named_storage;
    std::vector<std::string> names_list;
    std::vector<AbstractReference> numbered_storage;
    std::vector<boost::signals2::connection> signal_connections;
    std::vector<boost::optional<Type>> types;
};

} // namespace core

#endif
