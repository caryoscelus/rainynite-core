/*  link_storage.h - link storage
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

#ifndef CORE_NODE_LINK_STORAGE_H_56A4E74C_E276_5ADA_A9E9_06154E870892
#define CORE_NODE_LINK_STORAGE_H_56A4E74C_E276_5ADA_A9E9_06154E870892

#include <core/std/array.h>
#include <core/util/class_init.h>
#include <core/util/exceptions.h>
#include <core/node_info/node_info.h>
#include "abstract_list.h"
#include "abstract_value.h"
#include "make.h"

namespace rainynite::core {

/**
 * Typed storage of value node links.
 */
template <class Self, typename... Ts>
class LinkStorage : public AbstractListLinked
{
public:
    LinkStorage() {
        size_t i = 0;
        for (auto const& v : Self::default_values()) {
            set_link_without_checks(i, shallow_copy(*v));
            ++i;
        }
    }

    virtual ~LinkStorage() {
        for (auto& connection : signal_connections) {
            connection.disconnect();
        }
    }

    size_t link_count() const override {
        return sizeof...(Ts);
    }

    vector<AbstractReference> get_links() const override {
        return { storage.begin(), storage.end() };
    }

    AbstractReference get_link(size_t i) const override {
        check_range(i);
        if (auto link = storage[i])
            return link;
        throw NullPointerException("Null link");
    }

    TypeConstraint get_link_type(size_t i) const override {
        check_range(i);
        return types()[i];
    }

    void set_link(size_t i, AbstractReference value) override {
        check_range(i);
        auto const& t = types()[i];
        if (!(t.accept(value->get_type())))
            throw NodeAccessError("Node property type mis-match");
        set_link_without_checks(i, value);
        links_changed();
    }

private:
    void set_link_without_checks(size_t i, AbstractReference value) {
        signal_connections[i].disconnect();
        storage[i] = value;
        signal_connections[i] = value->subscribe([this](){
            links_changed();
        });
    }

public:
    template <typename... Is>
    void init_values(Is&&... values) {
        static_assert(sizeof...(Is) == sizeof...(Ts));
        storage = {make_value<Is>(std::forward<Is>(values))...};
        size_t i = 0;
        for (auto t : {std::type_index(typeid(Is))...}) {
            if (!types()[i].accept(t))
                throw NodeAccessError("Invalid type in init");
            ++i;
        }
    }

protected:
    /**
     * Called when links are changed.
     *
     * Mostly a hack to notify nodes of changes without inheriting it from here.
     * Used solely to call changed
     */
    virtual void links_changed() {
        link_change_signal();
    }

private:
    void check_range(size_t id) const {
        if (id >= sizeof...(Ts))
            throw std::out_of_range("set_link out of range");
    }

protected:
    using TypeArray = array<TypeConstraint, sizeof...(Ts)>;
    static TypeArray const& types() {
        static TypeArray instance { TypeConstraint(Ts())... };
        return instance;
    }

private:
    array<shared_ptr<AbstractValue>, sizeof...(Ts)> storage;
    array<boost::signals2::connection, sizeof...(Ts)> signal_connections;
};

template <typename... Ts>
vector<AbstractReference> generate_value_vector(Ts&&... values) {
    return {
        make_value<Ts>(std::forward<Ts>(values))...
    };
}

#define DEFAULT_VALUES(...) \
public: \
    static vector<AbstractReference> const& default_values() { \
        static vector<AbstractReference> instance { \
            generate_value_vector(__VA_ARGS__) \
        }; \
        return instance; \
    }

#define COMPLEX_DEFAULT_VALUES(...) \
public: \
    static vector<AbstractReference> const& default_values() { \
        static vector<AbstractReference> instance { \
            __VA_ARGS__ \
        }; \
        return instance; \
    }

} // namespace rainynite::core

#endif
