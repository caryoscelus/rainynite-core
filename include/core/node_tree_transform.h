/*  node_tree_transform.h - calculate transform
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

#ifndef CORE_NODE_TREE_TRANSFORM_H_5BE454D6_C46E_541A_A1A5_3EA70F024D0B
#define CORE_NODE_TREE_TRANSFORM_H_5BE454D6_C46E_541A_A1A5_3EA70F024D0B

#include <2geom/affine.h>

#include "node_tree.h"

namespace rainynite::core {

class TreeCalculateTransform : TREE_ELEMENT(TreeCalculateTransform) {
public:
    void added(NodeTree const& tree, NodeTree::Index index) override {
        if (auto prev_index = find_previous(tree, index)) {
            parent_element = tree.get_element<TreeCalculateTransform>(prev_index);
        }
        transform_node = tree.get_node_as<BaseValue<Geom::Affine>>(index);
    }

    Geom::Affine get_transform(shared_ptr<Context> ctx) const {
        if (parent_element)
            return parent_element->get_child_transform(ctx);
        return {};
    }

    Geom::Affine get_child_transform(shared_ptr<Context> ctx) const {
        return get_transform(ctx) * get_transform_change(ctx);
    }

    Geom::Affine get_transform_change(shared_ptr<Context> ctx) const {
        if (transform_node)
            return transform_node->value(ctx);
        return {};
    }

private:
    NodeTree::Index find_previous(NodeTree const& tree, NodeTree::Index index) const {
        if (index == nullptr || index->null() || index->root())
            return nullptr;
        auto parent = tree.parent(index);
        auto parent_value = tree.get_node(parent);
        // TODO: modularize?
        if (parent_value->get_type() == typeid(vector<Geom::Affine>)) {
            // assuming "transform composite"
            if (index->index > 0)
                return tree.index(parent, index->index-1);
            return parent;
        } else if (parent_value->get_type() == typeid(Geom::Affine)) {
            // no additional transform changes inside transform nodes
            return find_previous(tree, parent);
        } else if (auto parent_node = abstract_node_cast(parent_value)) {
            // TODO: check instead of try-catch
            size_t i;
            try {
                i = parent_node->get_name_id("transform");
            } catch (std::exception const& e) {
                return parent;
            }
            if (i != index->index)
                return tree.index(parent, i);
        }
        // parent isn't node or affine list - the only remaining possibility
        // is that it's heterogeneous (untyped) list
        return parent;
    }

private:
    observer_ptr<TreeCalculateTransform> parent_element;
    shared_ptr<BaseValue<Geom::Affine>> transform_node;
};

} // namespace rainynite::core

#endif
