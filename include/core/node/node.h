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

namespace core {

/**
 * Basic representation of any time-changeable Node
 */
template <typename T>
class Node : public BaseValue<T>, public AbstractNode {
public:
    template <typename U>
    void init(std::string const& name, U value) {
        init_property(name, boost::make_optional(Type(typeid(U))), make_value<U>(value));
    }
    template <typename U>
    void init_list(std::string const& name) {
        init_property(name, boost::make_optional(Type(typeid(std::vector<U>))), std::make_shared<ListValue<U>>());
    }
public:
    virtual std::string get_source_name() const {
        return "";
    }
    virtual bool can_set_source(std::shared_ptr<AbstractValue> src) const override {
        return !get_source_name().empty() && src->get_type() == this->get_type();
    }
    virtual void set_source(std::shared_ptr<AbstractValue> src) override {
        set_property(get_source_name(), src);
    }
};

} // namespace core

#endif
