/*  node_tree_transform.cpp - calculate transform
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

#include <core/node/value.h>
#include <core/node/abstract_node.h>
#include <core/node_tree/transform.h>

namespace rainynite::core {

class TreeCalculateTransform : TREE_ELEMENT(TreeCalculateTransform) {
public:
    void added(NodeTree const& tree, NodeTree::Index index) override;

    Geom::Affine get_transform(shared_ptr<Context> ctx) const;

    Geom::Affine get_child_transform(shared_ptr<Context> ctx) const;

    Geom::Affine get_transform_change(shared_ptr<Context> ctx) const;

private:
    NodeTree::Index find_previous(NodeTree const& tree, NodeTree::Index index) const;

private:
    observer_ptr<TreeCalculateTransform> parent_element;
    shared_ptr<BaseValue<Geom::Affine>> transform_node;
};

void TreeCalculateTransform::added(NodeTree const& tree, NodeTree::Index index) {
    if (auto prev_index = find_previous(tree, index)) {
        parent_element = tree.get_element<TreeCalculateTransform>(prev_index);
    }
    transform_node = tree.get_node_as<BaseValue<Geom::Affine>>(index);
}

Geom::Affine TreeCalculateTransform::get_transform(shared_ptr<Context> ctx) const {
    if (parent_element)
        return parent_element->get_child_transform(ctx);
    return {};
}

Geom::Affine TreeCalculateTransform::get_child_transform(shared_ptr<Context> ctx) const {
    return get_transform(ctx) * get_transform_change(ctx);
}

Geom::Affine TreeCalculateTransform::get_transform_change(shared_ptr<Context> ctx) const {
    if (transform_node)
        return transform_node->value(ctx);
    return {};
}

NodeTree::Index TreeCalculateTransform::find_previous(NodeTree const& tree, NodeTree::Index index) const {
    auto parent = tree.parent(index);
    if (!parent)
        return parent;
    auto parent_value = tree.get_node(parent);
    auto link_index = tree.link_index(index);

    // TODO: modularize?
    if (parent_value->get_type() == typeid(vector<Geom::Affine>)) {
        // assuming "transform composite"
        if (link_index < tree.children_count(parent)-1)
            return tree.index(parent, link_index+1);
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
        if (i != link_index)
            return tree.index(parent, i);
    }
    // parent isn't node or affine list - the only remaining possibility
    // is that it's heterogeneous (untyped) list
    return parent;
}

Geom::Affine get_transform(shared_ptr<Context> ctx, NodeTree const& tree, NodeTree::Index index) {
    if (auto calculate_tr = tree.get_element<TreeCalculateTransform>(index))
        return calculate_tr->get_transform(ctx);
    return {};
}

} // namespace rainynite::core
