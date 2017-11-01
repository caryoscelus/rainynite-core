/*  new_node.h - new node
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

#ifndef CORE_NODE_NEW_NODE_H_C1EE08BB_8FA1_5CA3_9EF7_FCF458718F5E
#define CORE_NODE_NEW_NODE_H_C1EE08BB_8FA1_5CA3_9EF7_FCF458718F5E

#include "named_link_storage.h"

namespace rainynite::core {

template <class Self, typename Result, typename... Ts>
class NewNode :
    public NamedLinkStorage<Self, Ts...>,
    public DocString,
    public BaseValue<Result>
{
    using Storage = NamedLinkStorage<Self, Ts...>;
public:
    bool can_set_source(shared_ptr<AbstractValue> src) const override {
        if (this->link_count() == 0)
            return false;
        return this->get_link_type(0).accept(src->get_type());
    }
    void set_source(shared_ptr<AbstractValue> src) override {
        this->set_link(0, src);
    }
    AbstractReference get_property(string const& name) const override {
        if (is_custom_property(name))
            return get_custom_property(name);
        return Storage::get_property(name);
    }
    void set_property(string const& name, AbstractReference ref) override {
        if (is_custom_property(name))
            set_custom_property(name, ref);
        else
            Storage::set_property(name, ref);
    }
    bool remove_property(string const& name) override {
        if (is_custom_property(name))
            return remove_custom_property(name);
        else
            return Storage::remove_property(name);
    }

protected:
    void node_changed() override {
        this->changed();
    }

    void links_changed() override {
        this->changed();
    }

private:
    bool is_custom_property(string const& name) const {
        return !name.empty() && name[0] == '_';
    }

    AbstractReference get_custom_property(string const& name) const {
        auto it = cp_names.find(name);
        if (it != cp_names.end())
            return cp_storage[it->second];
        throw NodeAccessError("Custom property '"+name+"' not found");
    }

    void set_custom_property(string const& name, AbstractReference ref) {
        auto it = cp_names.find(name);
        if (it != cp_names.end()) {
            cp_storage[it->second] = ref;
        } else {
            cp_names.emplace(name, cp_storage.size());
            cp_storage.push_back(ref);
        }
    }

    bool remove_custom_property(string const& name) {
        auto it = cp_names.find(name);
        if (it != cp_names.end()) {
            cp_storage.erase(cp_storage.begin()+it->second);
            cp_names.erase(it);
            return true;
        }
        return false;
    }

private:
    vector<AbstractReference> cp_storage;
    map<string, size_t> cp_names;
};

} // namespace rainynite::core

#endif
