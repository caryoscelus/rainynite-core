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
public:
    bool can_set_source(shared_ptr<AbstractValue> src) const override {
        if (this->link_count() == 0)
            return false;
        return this->get_link_type(0).accept(src->get_type());
    }
    void set_source(shared_ptr<AbstractValue> src) override {
        this->set_link(0, src);
    }
protected:
    void node_changed() override {
        this->changed();
    }
};

} // namespace rainynite::core

#endif
