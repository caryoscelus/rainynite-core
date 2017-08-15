/*
 *  abstract_list.h - abstract list
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

#ifndef __CORE__NODE__ABSTRACT_LIST_H__A2D19F78
#define __CORE__NODE__ABSTRACT_LIST_H__A2D19F78

#include "common.h"
#include "node_in_context.h"

namespace core {

class AbstractListLinked {
public:
    virtual size_t link_count() const = 0;
    virtual std::vector<AbstractReference> get_links() const = 0;
    virtual std::vector<NodeInContext> get_list_links(std::shared_ptr<Context>) const = 0;
    virtual AbstractReference get_link(size_t i) const = 0;
    virtual boost::optional<Type> get_link_type(size_t i) const = 0;
    virtual void set_link(size_t i, AbstractReference value) = 0;
    virtual void push_back(AbstractReference) {
        throw NodeAccessError("cannot push back");
    }
    virtual void push_new() {
        throw NodeAccessError("cannot push back");
    }
    virtual void remove(size_t) {
        throw NodeAccessError("cannot remove");
    }
    virtual bool is_editable_list() const {
        return false;
    }
public:
    template <class T>
    BaseReference<T> get_link_as(size_t i) const {
        return std::dynamic_pointer_cast<BaseValue<T>>(get_link(i));
    }
};

using AbstractListReference = std::shared_ptr<AbstractListLinked>;

} // namespace core

#endif
