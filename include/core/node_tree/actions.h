/*  node_tree/actions.h - Node tree helper functions
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

#ifndef CORE_NODE_TREE_ACTIONS_H_B6A262F8_6822_5AEC_820E_9180B17D107B
#define CORE_NODE_TREE_ACTIONS_H_B6A262F8_6822_5AEC_820E_9180B17D107B

#include "node_tree.h"

namespace rainynite::core {

/// Get node tree index of named property of `parent` node
NodeTree::Index index_of_property(NodeTree const& tree, NodeTree::Index parent, string const& name);

/// Put `value` at `index` in `tree`
void replace_index(NodeTree& tree, NodeTree::Index index, AbstractReference value);

/// Add custom property to a node
NodeTree::Index add_custom_property(NodeTree& tree, NodeTree::Index parent, string const& name, AbstractReference value);

/// Push new element to a list
NodeTree::Index push_new_to(NodeTree& tree, NodeTree::Index parent);

/// Push element to a list
void push_to(NodeTree& tree, NodeTree::Index parent, AbstractReference value);

/// Insert element to a list
NodeTree::Index insert_to(NodeTree& tree, NodeTree::Index parent, size_t position, AbstractReference value);

/// Pop from list and return the element
AbstractReference pop_from(NodeTree& tree, NodeTree::Index parent);

/// Remove list item at index
void remove_index(NodeTree& tree, NodeTree::Index index);

/// Replace node with its first child
void replace_with_child(NodeTree& tree, NodeTree::Index index);

} // namespace rainynite::core

#endif
