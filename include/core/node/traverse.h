/*
 *  traverse.h - node traversing
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

#ifndef CORE_NODE_TRAVERSE_H_CC85EE3C_3B5E_5268_9929_8FC36285B95C
#define CORE_NODE_TRAVERSE_H_CC85EE3C_3B5E_5268_9929_8FC36285B95C

#include <list>

#include <core/std/set.h>
#include "abstract_list.h"

namespace rainynite::core {

enum class TraverseDepth {
    Once,
    Deeper
};

template <typename T>
T traverse_once(AbstractReference root, std::function<optional<T>(AbstractReference)> f, TraverseDepth depth = TraverseDepth::Once) {
    set<AbstractReference> traversed;
    std::list<AbstractReference> to_traverse;
    to_traverse.push_back(root);
    while (!to_traverse.empty()) {
        auto i = to_traverse.begin();
        auto node = *i;
        to_traverse.erase(i);
        if (traversed.count(node) > 0 && depth == TraverseDepth::Once)
            continue;

        if (optional<T> result = f(node))
            return *result;

        if (traversed.count(node) == 0) {
            if (auto linked_node = dynamic_pointer_cast<AbstractListLinked>(node)) {
                auto links = linked_node->get_links();
                std::copy_if(
                    links.begin(),
                    links.end(),
                    std::back_inserter(to_traverse),
                    [&traversed, depth](AbstractReference ref) {
                        return depth == TraverseDepth::Deeper
                            || traversed.count(ref) == 0;
                    }
                );
            }
        }
        traversed.insert(node);
    }
    return {};
}

} // namespace rainynite::core

#endif
