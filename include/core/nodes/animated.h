/*
 *  animated.h - node that switches between its children in time
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

#ifndef __CORE__NODES__ANIMATED_H__28E7E2B8
#define __CORE__NODES__ANIMATED_H__28E7E2B8

#include <core/node.h>
#include <core/time/time_period.h>

namespace core {
namespace nodes {

class AbstractAnimated {
public:
    virtual void add_child(TimePeriod period, AbstractReference ref) = 0;
    virtual size_t child_count() const = 0;
};

} // namespace nodes
} // namespace core

#endif
