/*  named_link_storage.h - link storage
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

#ifndef CORE_NODE_NAMED_LINK_STORAGE_H_067842B0_98D7_5D5D_B8A3_0C1D771E99AC
#define CORE_NODE_NAMED_LINK_STORAGE_H_067842B0_98D7_5D5D_B8A3_0C1D771E99AC

#include <core/std/map.h>
#include "link_storage.h"
#include "abstract_node.h"

namespace rainynite::core {

inline map<string, size_t> get_name_map(vector<string> const& names) {
    // TODO: algorithm?
    map<string, size_t> result;
    size_t i = 0;
    for (auto const& s : names) {
        result[s] = i;
        ++i;
    }
    return result;
}

template <class Self, typename... Ts>
class NamedLinkStorage :
    public LinkStorage<Self, Ts...>,
    public AbstractNode
{
public:
    static size_t get_name_id_s(string const& name) {
        auto const& names = name_map();
        auto it = names.find(name);
        if (it != names.end())
            return it->second;
        throw NodeAccessError("Property doesn't exist");
    }

    size_t get_name_id(string const& name) const override {
        return get_name_id_s(name);
    }

    AbstractReference get_property(string const& name) const override {
        return this->get_link(get_name_id_s(name));
    }

    void set_property(string const& name, AbstractReference ref) override {
        this->set_link(get_name_id_s(name), ref);
    }

    map<string, AbstractReference> get_link_map() const override {
        map<string, AbstractReference> result;
        auto const& names = Self::link_names();
        std::transform(
            names.begin(),
            names.end(),
            std::inserter(result, result.end()),
            [this](auto const& name) -> decltype(result)::value_type {
                return { name, get_property(name) };
            }
        );
        return result;
    }

    string get_name_at(size_t id) const override {
        return Self::link_names().at(id);
    }

protected:
    void node_changed() override {
    }

private:
    static map<string, size_t> const& name_map() {
        static map<string, size_t> instance(get_name_map(Self::link_names()));
        return instance;
    }
};

#define NODE_PROPERTIES(...) \
public: \
    static vector<string> const& link_names() { \
        static vector<string> instance { __VA_ARGS__ }; \
        return instance; \
    }

#define PROPERTY(name) \
private: \
    size_t name##_id() const { \
        static size_t id { this->get_name_id(#name) }; \
        return id; \
    } \
public: \
    AbstractReference p_##name() const { \
        auto id = name##_id(); \
        return this->get_link(id); \
    } \
    template <typename PROPERTY_T> \
    PROPERTY_T name##_value(shared_ptr<Context> ctx) const { \
        auto id = name##_id(); \
        if (!this->types()[id].accept(typeid(PROPERTY_T))) \
            throw NodeAccessError("Property cannot contain requested type."); \
        p_##name(); \
        if (auto p = this->template get_link_as<PROPERTY_T>(id)) \
            return p->value(ctx); \
        throw NodeAccessError("Property doesn't contain value of requested type."); \
    }

} // namespace rainynite::core

#endif
