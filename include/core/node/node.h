/*  node.h - Node
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

#ifndef CORE_NODE_NODE_H_476155BB_19F8_5EF7_9AC8_30BEA7394210
#define CORE_NODE_NODE_H_476155BB_19F8_5EF7_9AC8_30BEA7394210

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
        init_property(name, Type(typeid(U)), make_value<U>(value));
    }
    template <typename U>
    void init_list(string const& name) {
        auto list = make_shared<ListValue<U>>();
        list->new_id();
        init_property(name, Type(typeid(vector<U>)), list);
    }
public:
    bool can_set_source(shared_ptr<AbstractValue> src) const override {
        if (link_count() == 0)
            return false;
        return get_link_type(0).accept(src->get_type());
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
