/*  node_tree/io.h - node tree related io
 *  Copyright (C) 2018 caryoscelus
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

#ifndef CORE_NODE_TREE_IO_H_D5C4B38A_7F64_557C_9B72_B2D3CFA0BFE8
#define CORE_NODE_TREE_IO_H_D5C4B38A_7F64_557C_9B72_B2D3CFA0BFE8

#include <iostream>

#include "path.h"

namespace rainynite::core {

std::ostream& operator<<(std::ostream& out, NodeTreePath const& path);

} // namespace rainynite::core

#endif
