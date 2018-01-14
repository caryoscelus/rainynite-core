/*  new_node.h - new node
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
    Result default_value(shared_ptr<Context> ctx) const noexcept override {
        if (this->link_count() == 0)
            return {};
        if (auto link = this->template get_link_as<Result>(0)) {
            return link->value(ctx);
        }
        return {};
    }
    size_t get_name_id(string const& name) const override {
        if (is_custom_property(name))
            return get_custom_property_id(name);
        return Storage::get_name_id(name);
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
    void remove(size_t i) override {
        auto name = get_name_at(i);
        if (is_custom_property(name))
            remove_custom_property(name);
        else
            Storage::remove(i);
    }
    size_t link_count() const override {
        return Storage::link_count()+cp_storage.size();
    }
    map<string, AbstractReference> get_link_map() const override {
        auto result = Storage::get_link_map();
        if (!has_custom_properties())
            return result;
        for (auto const& e : cp_names) {
            result.emplace(e.first, cp_storage[e.second]);
        }
        return result;
    }
    vector<AbstractReference> get_links() const override {
        auto result = Storage::get_links();
        if (!has_custom_properties())
            return result;
        result.insert(result.end(), cp_storage.begin(), cp_storage.end());
        return result;
    }
    string get_name_at(size_t id) const override {
        return apply_for_normal_or_custom<string>(
            id,
            [this](auto i) {
                return Storage::get_name_at(i);
            },
            [this](auto i) {
                return cp_name_list[i];
            }
        );
    }
    AbstractReference get_link(size_t i) const override {
        return apply_for_normal_or_custom<AbstractReference>(
            i,
            [this](auto i) {
                return Storage::get_link(i);
            },
            [this](auto i) {
                return cp_storage[i];
            }
        );
    }
    TypeConstraint get_link_type(size_t i) const override {
        return apply_for_normal_or_custom<TypeConstraint>(
            i,
            [this](auto i) {
                return Storage::get_link_type(i);
            },
            [](auto /*i*/) {
                return types::Any();
            }
        );
    }
    void set_link(size_t i, AbstractReference value) override {
        return apply_for_normal_or_custom<void>(
            i,
            [this, value](auto ii) {
                Storage::set_link(ii, value);
            },
            [this, value](auto ii) {
                cp_storage[ii] = value;
            }
        );
    }

protected:
    void node_changed() override {
        this->changed();
    }

    void links_changed() override {
        Storage::links_changed();
        this->changed();
    }

private:
    template <typename T, class F, class G>
    T apply_for_normal_or_custom(size_t i, F normal, G custom) const {
        auto cp_id = (ptrdiff_t)i - (ptrdiff_t)Storage::link_count();
        if (cp_id < 0)
            return normal(i);
        size_t id = cp_id;
        if (id >= cp_storage.size())
            throw std::out_of_range("Link id out of range");
        return custom(id);
    }

    bool is_custom_property(string const& name) const {
        return !name.empty() && name[0] == '_';
    }

    size_t get_custom_property_id(string const& name) const {
        return Storage::link_count()+cp_names.at(name);
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
            cp_name_list.push_back(name);
            cp_storage.push_back(ref);
        }
    }

    bool remove_custom_property(string const& name) {
        auto it = cp_names.find(name);
        if (it != cp_names.end()) {
            auto id = it->second;
            cp_storage.erase(cp_storage.begin() + id);
            cp_name_list.erase(cp_name_list.begin() + id);
            cp_names.erase(it);
            for (auto& e : cp_names) {
                if (e.second > id)
                    --e.second;
            }
            return true;
        }
        return false;
    }

    bool has_custom_properties() const {
        return cp_storage.size() != 0;
    }

private:
    vector<AbstractReference> cp_storage;
    map<string, size_t> cp_names;
    vector<string> cp_name_list;
};

} // namespace rainynite::core

#endif
