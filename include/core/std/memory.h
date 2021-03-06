/*  std/memory.h - re-import memory
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

#ifndef CORE_STD_MEMORY_H_FF669312_947A_59A4_929E_E7C040C4BDBC
#define CORE_STD_MEMORY_H_FF669312_947A_59A4_929E_E7C040C4BDBC

#include <memory>
#include <experimental/memory>

namespace rainynite {

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::experimental::observer_ptr;

using std::dynamic_pointer_cast;
using std::static_pointer_cast;
using std::const_pointer_cast;
// using std::reinterpret_pointer_cast; // not used yet

using std::enable_shared_from_this;
using std::make_unique;
using std::make_shared;
using std::experimental::make_observer;

} // namespace rainynite

#endif
