/*
 *  node.h - Node
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

#ifndef __CORE__NODE__NODE_H__245D750C
#define __CORE__NODE__NODE_H__245D750C

#include "abstract_node.h"
#include "abstract_value.h"
#include "list.h"

namespace rainynite::core {

/**
 * Basic representation of any time-changeable Node
 */
template <typename T>
class Node : public BaseValue<T>, public AbstractNode {
public:
    template <typename U>
    void init(string const& name, U value) {
        init_property(name, make_optional(Type(typeid(U))), make_value<U>(value));
    }
    template <typename U>
    void init_list(string const& name) {
        init_property(name, make_optional(Type(typeid(vector<U>))), std::make_shared<ListValue<U>>());
    }
public:
    bool can_set_source(shared_ptr<AbstractValue> src) const override {
        if (link_count() == 0)
            return false;
        if (auto type = get_link_type(0))
            return src->get_type() == *type;
        return true;
    }
    void set_source(shared_ptr<AbstractValue> src) override {
        set_link(0, src);
    }
protected:
    void node_changed() override {
        this->changed();
    }
};

} // namespace rainynite::core

#endif
