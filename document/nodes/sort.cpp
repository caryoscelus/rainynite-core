/*  sort.cpp - sorting nodes
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

#include <core/std/algorithm.h>
#include <core/node_info/macros.h>
#include <core/node/property.h>
#include <core/node/new_node.h>
#include <core/node/list.h>
#include <core/node/replace_context.h>
#include <core/all_types.h>
#include <core/util/nullptr.h>

namespace rainynite::core {

template <typename T>
class Sort :
    public NewNode<
        Sort<T>,
        vector<T>,
        types::Only<vector<T>>,
        types::Only<bool>
    >
{
    DOC_STRING(
        "Sort typed list with sorting node."
    )

    NODE_PROPERTIES("source", "sort_by")
    COMPLEX_DEFAULT_VALUES(make_node<ListValue<T>>(), make_node_with_name("Less/"+get_primitive_type_name<T>()))
    PROPERTY(source)
    PROPERTY(sort_by)

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        auto links = p_source()->list_links(ctx);
        auto less = shallow_copy(*p_sort_by());
        auto less_node = no_null(abstract_node_cast(less));
        std::sort(
            links.begin(),
            links.end(),
            [less, less_node, ctx](auto a, auto b) {
                try {
                    less_node->set_property("a", make_replace_context_node(typeid(T), a));
                    less_node->set_property("b", make_replace_context_node(typeid(T), b));
                    return any_cast<bool>(less->get_any(ctx));
                } catch (...) {
                    return true;
                }
            }
        );
        return links;
    }

public:
    size_t list_links_count(shared_ptr<Context> ctx) const noexcept override {
        return p_source()->list_links_count(ctx);
    }
};

NODE_INFO_INSTANCES(Sort, Sort<T>, vector<T>)

} // namespace rainynite::core
