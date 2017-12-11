/*  abstract_list.h - abstract list
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

#ifndef CORE_NODE_ABSTRACT_LIST_H_CC2AFA00_E2FA_5559_A49F_9E4CCC696C5D
#define CORE_NODE_ABSTRACT_LIST_H_CC2AFA00_E2FA_5559_A49F_9E4CCC696C5D

#include <core/std/vector.h>
#include "type_constraint.h"
#include "common.h"
#include "node_in_context.h"
#include "cast.h"

namespace rainynite::core {

/**
 * Abstract sequence of links to nodes.
 */
class AbstractListLinked {
public:
    virtual ~AbstractListLinked() {}

    virtual size_t link_count() const = 0;
    virtual vector<AbstractReference> get_links() const = 0;
    virtual AbstractReference get_link(size_t i) const = 0;

    /// Returns link type constraint
    virtual TypeConstraint get_link_type(size_t i) const = 0;

    virtual void set_link(size_t i, AbstractReference value) = 0;

    virtual void push_back(AbstractReference) {
        throw NodeAccessError("cannot push back");
    }
    virtual void push_new() {
        throw NodeAccessError("cannot push new");
    }
    virtual void insert(size_t /*i*/, AbstractReference /*value*/) {
        throw NodeAccessError("cannot insert");
    }
    virtual void remove(size_t /*i*/) {
        throw NodeAccessError("cannot remove");
    }
    virtual void pop() {
        throw NodeAccessError("cannot pop");
    }

    /// Returns true if this is "normal list", i.e. its value consists of its links
    virtual bool is_editable_list() const {
        return false;
    }

public:
    template <class T>
    shared_ptr<BaseValue<T>> get_link_as(size_t i) const {
        return base_value_cast<T>(get_link(i));
    }
};

} // namespace rainynite::core

#endif
