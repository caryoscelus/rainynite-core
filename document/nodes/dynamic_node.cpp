/*  dynamic_node.cpp - create node from argument list
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

#include <core/util/nullptr.h>
#include <core/node_info/macros.h>
#include <core/node_info/copy.h>
#include <core/node_info/default_node.h>
#include <core/node/proxy_node.h>
#include <core/node/list.h>
#include <core/node/replace_context.h>
#include <core/all_types.h>

namespace rainynite::core::nodes {

template <typename T>
class DynamicNode :
    public NewProxyNode<
        DynamicNode<T>,
        T,
        types::Only<string>,
        types::Only<Nothing> // TODO: better list typing
    >
{
    DOC_STRING("Create node from name and arguments")

    NODE_PROPERTIES("node_type", "arguments")
    COMPLEX_DEFAULT_VALUES(make_value<string>(), make_node<UntypedListValue>())
    PROPERTY(node_type)
    PROPERTY(arguments)

public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto type = node_type_value<string>(ctx);
        if (cached_type != type) {
            node = make_node_with_name_as<AbstractValue>(type);
            cached_type = type;
        }
        size_t i = 0;
        auto node_list = dynamic_cast<AbstractListLinked*>(node.get());
        if (auto args = this->get_property("arguments")) {
            for (auto const& arg : args->list_links(ctx)) {
                // TODO: context
                node_list->set_link(i, arg.node);
                ++i;
            }
            return {node, ctx};
        } else {
            throw NodeAccessError("DynamicNode: arguments property is null");
        }
    }

private:
    mutable AbstractReference node;
    mutable string cached_type;
};

NODE_INFO_TEMPLATE(DynamicNode, DynamicNode<T>, T);
TYPE_INSTANCES(DynamicNodeNodeInfo)


template <typename T>
class ApplyToList :
    public NewNode<
        ApplyToList<T>,
        vector<T>,
        types::Only<T>,
        types::Only<string>,
        types::Only<Nothing>, // TODO
        types::Only<bool>
    >
{
    DOC_STRING(
        "Convert list by applying node to each element.\n"
        "\n"
        "Returns list, each element of which is node with a few common properties\n"
        "and one from input list."
    )

    NODE_PROPERTIES("source", "property_name", "dynamic_arguments", "use_dynamic_context")
    COMPLEX_DEFAULT_VALUES(make_default_node<T>(), make_value<string>(), make_node<UntypedListValue>(), make_value<bool>(true))
    PROPERTY(source)
    PROPERTY(property_name)
    PROPERTY(dynamic_arguments)
    PROPERTY(use_dynamic_context)

public:
    size_t list_links_count(shared_ptr<Context> ctx) const override {
        if (auto list = p_dynamic_arguments())
            return list->list_links_count(ctx);
        return 0;
    }

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        vector<NodeInContext> result;
        auto property = property_name_value<string>(ctx);
        auto base_node = p_source();
        auto dy_args = p_dynamic_arguments()->list_links(ctx);
        bool using_dynamic_context = use_dynamic_context_value<bool>(ctx);
        std::transform(
            std::begin(dy_args),
            std::end(dy_args),
            std::back_inserter(result),
            [base_node, property, ctx, using_dynamic_context](NodeInContext e) -> NodeInContext {
                auto node = shallow_copy(*base_node);
                auto prop = using_dynamic_context ? e.node : make_replace_context_node(e.node->get_type(), e);
                abstract_node_cast(node)->set_property(property, prop);
                auto context = using_dynamic_context ? e.context : ctx;
                return { node, context };
            }
        );
        return result;
    }
};

NODE_INFO_TEMPLATE(ApplyToList, ApplyToList<T>, vector<T>);
TYPE_INSTANCES(ApplyToListNodeInfo)


template <typename T>
class DynamicListZip :
    public NewNode<
        DynamicListZip<T>,
        vector<T>,
        types::Only<string>,
        types::Only<Nothing>
    >
{
    DOC_STRING(
        "Zip lists of lists into a list of nodes with args.\n"
        "\n"
        "Takes few homogeneous lists and uses their elements as node properties\n"
        "(each list representing one property) in new node list."
    )

    NODE_PROPERTIES("node_type", "arguments_list")
    COMPLEX_DEFAULT_VALUES(make_value<string>(), make_node<UntypedListValue>())
    PROPERTY(node_type)
    PROPERTY(arguments_list)

public:
    size_t list_links_count(shared_ptr<Context> ctx) const override {
        auto args = no_null(this->get_property("arguments_list"));
        auto list_of_lists = args->list_links(ctx);
        if (list_of_lists.size() == 0)
            return 0;
        auto iter = list_of_lists.begin();
        size_t result = iter->node->list_links_count(iter->context);
        while (++iter != list_of_lists.end() && result != 0) {
            result = std::min(result, iter->node->list_links_count(iter->context));
        }
        return result;
    }

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        using List = vector<NodeInContext>;
        using Iter = List::const_iterator;
        auto args = this->get_property("arguments_list");
        if (!args)
            throw NodeAccessError("arguments list is null");
        auto list_of_lists = args->list_links(ctx);
        if (list_of_lists.size() == 0)
            return {};
        vector<List> links;
        vector<pair<Iter, Iter>> iterators;
        bool fail = false;
        std::transform(
            std::begin(list_of_lists),
            std::end(list_of_lists),
            std::back_inserter(links),
            [&fail](auto e) {
                try {
                    return e.node->list_links(e.context);
                } catch (...) {
                }
                fail = true;
                return List();
            }
        );
        if (fail) {
            return {};
            // throw
        }
        std::transform(
            std::begin(links),
            std::end(links),
            std::back_inserter(iterators),
            [](auto const& list) -> pair<Iter, Iter> {
                return { std::begin(list), std::end(list) };
            }
        );
        vector<NodeInContext> result;
        auto type = node_type_value<string>(ctx);
        while (true) {
            auto node = make_node_with_name_as<AbstractValue>(type);
            auto list_node = dynamic_cast<AbstractListLinked*>(node.get());
            if (!list_node)
                return result;
            size_t i = 0;
            for (auto& e : iterators) {
                if (e.first == e.second)
                    return result;
                list_node->set_link(i, make_replace_context_node(e.first->node->get_type(), *e.first));
                ++e.first;
                ++i;
            }
            result.emplace_back(node, ctx);
        }
    }
};

NODE_INFO_TEMPLATE(DynamicListZip, DynamicListZip<T>, vector<T>);
TYPE_INSTANCES(DynamicListZipNodeInfo)


} // namespace rainynite::core::nodes
