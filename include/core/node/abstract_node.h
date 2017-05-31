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

#include "abstract_list.h"

namespace core {

class AbstractNode : public AbstractListLinked {
public:
    AbstractReference get_property(std::string const& name) const {
        auto result = named_storage.find(name);
        if (result == named_storage.end())
            throw NodeAccessError("Unknown property "+name);
        return get_by_id(result->second);
    }
    template <typename T>
    BaseReference<T> get_property_as(std::string const& name) const {
        return std::dynamic_pointer_cast<BaseValue<T>>(get_property(name));
    }
    template <typename T>
    boost::optional<T> get_property_value(std::string const& name, Time time) const {
        try {
            return get_property_as<T>(name)->get(time);
        } catch (NodeAccessError const&) {
            return boost::none;
        } catch (...) {
            return boost::none;
        }
    }
    void set_property(std::string const& name, AbstractReference ref) {
        if (named_storage.count(name) == 0) {
            if (name[0] == '_') {
                // accept as custom attribute
                init_property(name, boost::none, ref);
            } else
                throw NodeAccessError("No such property");
        }
        get_by_id(named_storage[name]) = ref;
    }
    size_t init_property(std::string const& name, boost::optional<Type> type, AbstractReference value) {
        size_t id = link_count();
        numbered_storage.push_back(std::move(value));
        names_list.push_back(name);
        named_storage[name] = id;
        types.push_back(type);
        return id;
    }
    std::map<std::string, AbstractReference> get_link_map() const {
        std::map<std::string, AbstractReference> result;
        // TODO: use generic conversion function
        for (auto const& e : named_storage) {
            result.emplace(e.first, get_by_id(e.second));
        }
        return result;
    }
    std::string get_name_at(size_t id) {
        return names_list[id];
    }
public:
    virtual std::vector<AbstractReference> get_links() const override {
        return numbered_storage;
    }
    virtual AbstractReference get_link(size_t i) const override {
        return get_by_id(i);
    }
    virtual boost::optional<Type> get_link_type(size_t i) const override {
        return types[i];
    }
    virtual void set_link(size_t i, AbstractReference value) override {
        if (auto type = get_link_type(i)) {
            if (value->get_type() != type.get())
                throw NodeAccessError("Node property type mis-match");
        }
        get_by_id(i) = value;
    }
    virtual size_t link_count() const override {
        return numbered_storage.size();
    }
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
    std::vector<boost::optional<Type>> types;
};

} // namespace core

#endif
