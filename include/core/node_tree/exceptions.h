/*  node_tree/exceptions.h - Node tree exceptions
 *  Copyright (C) 2017-2018 caryoscelus
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

#ifndef CORE_NODE_TREE_EXCEPTIONS_H_5E1FA1DD_893D_5530_BA12_73526720FC04
#define CORE_NODE_TREE_EXCEPTIONS_H_5E1FA1DD_893D_5530_BA12_73526720FC04

namespace rainynite::core {

struct NodeTreeError : public std::runtime_error {
    template <typename... Args>
    NodeTreeError(Args&&... args) :
        std::runtime_error(std::forward<Args>(args)...)
    {}
};

struct InvalidIndexError : public NodeTreeError {
    template <typename... Args>
    InvalidIndexError(Args&&... args) :
        NodeTreeError(std::forward<Args>(args)...)
    {}
};

struct TreeCorruptedError : public NodeTreeError {
    template <typename... Args>
    TreeCorruptedError(Args&&... args) :
        NodeTreeError(std::forward<Args>(args)...)
    {}
};

} // namespace rainynite::core

#endif
