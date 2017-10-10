/*  type_constraint.h - type constraint class
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

#ifndef CORE_NODE_TYPE_CONSTRAINT_H_DD439EC1_C0E3_56F2_B114_4ED9D33CB8D0
#define CORE_NODE_TYPE_CONSTRAINT_H_DD439EC1_C0E3_56F2_B114_4ED9D33CB8D0

#include <core/std/set.h>
#include <core/std/type.h>

namespace rainynite::core {

struct TypeConstraint {
    explicit TypeConstraint(set<Type> accepted_) :
        accepted(accepted_)
    {}
    TypeConstraint(Type t) :
        accepted({t})
    {}
    TypeConstraint()
    {}
    bool operator==(TypeConstraint const& other) {
        return accepted == other.accepted;
    }
    bool accept(Type type) const {
        return accepted.empty() || accepted.find(type) != accepted.end();
    }
    set<Type> accepted;
};

template <typename T>
struct Only {
    operator Type() {
        return typeid(T);
    }
};

} // namespace rainynite::core

#endif
