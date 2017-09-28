/*
 *  dynamic_node.cpp - create node from argument list
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

#include <core/node_info.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/node/proxy_node.h>
#include <core/all_types.h>

namespace rainynite::core {
namespace nodes {

template <typename T>
class DynamicNode : public ProxyNode<T> {
public:
    DynamicNode() {
        this->template init<string>(node_type, {});
        auto args = make_shared<UntypedListValue>();
        args->new_id();
        this->template init_property(arguments, make_optional(Type(typeid(Nothing))), std::move(args));
    }
public:
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        try {
            auto type = get_node_type()->get(ctx);
            if (cached_type != type) {
                node = make_node_with_name<AbstractValue>(type);
                cached_type = type;
            }
            size_t i = 0;
            auto node_list = dynamic_cast<AbstractListLinked*>(node.get());
            if (auto args = this->get_property("arguments")) {
                for (auto const& arg : args->get_list_links(ctx)) {
                    // TODO: context
                    node_list->set_link(i, arg.node);
                    ++i;
                }
                return {node, ctx};
            } else {
                throw NodeAccessError("DynamicNode: arguments property is null");
            }
        } catch (...) {
            return { make_value<T>(), ctx };
        }
    }

private:
    mutable AbstractReference node;
    mutable string cached_type;

private:
    NODE_PROPERTY(node_type, string);
    NODE_LIST_PROPERTY(arguments, Nothing);
};

NODE_INFO_TEMPLATE(DynamicNode, DynamicNode<T>, T);
TYPE_INSTANCES(DynamicNodeNodeInfo)

/**
 * Convert list by applying node to each element.
 *
 * Returns list, each element of which is node with a few common properties
 * and one from input list.
 */
template <typename T>
class ApplyToList : public ProxyListNode<T> {
public:
    ApplyToList() {
        this->template init<T>(source, {});
        this->template init<string>(property_name, {});
        // TODO: make a function
        {
            auto args = make_shared<UntypedListValue>();
            args->new_id();
            this->template init_property(dynamic_arguments, make_optional(Type(typeid(Nothing))), std::move(args));
        }
    }
public:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        vector<NodeInContext> result;
        try {
            auto property = get_property_name()->get(ctx);
            auto base_node = get_source();
            auto dy_args = this->get_property(dynamic_arguments)->get_list_links(ctx);
            std::transform(
                std::begin(dy_args),
                std::end(dy_args),
                std::back_inserter(result),
                [base_node, property](auto e) -> NodeInContext {
                    auto node = shallow_copy(*base_node);
                    // TODO: fix context
                    dynamic_cast<AbstractNode*>(node.get())->set_property(property, e.node);
                    return { node, e.context };
                }
            );
        } catch (...) {
        }
        return result;
    }
private:
    NODE_PROPERTY(source, T);
    NODE_PROPERTY(property_name, string);
    NODE_LIST_PROPERTY(dynamic_arguments, Nothing);
};

NODE_INFO_TEMPLATE(ApplyToList, ApplyToList<T>, vector<T>);
TYPE_INSTANCES(ApplyToListNodeInfo)

/**
 * Zip lists of lists into a list of nodes with args.
 *
 * Takes few homogeneous lists and uses their elements as node properties
 * (each list representing one property) in new node list.
 */
template <typename T>
class DynamicListZip : public ProxyListNode<T> {
public:
    DynamicListZip() {
        this->template init<string>(node_type, {});
        auto args = make_shared<UntypedListValue>();
        args->new_id();
        this->template init_property(arguments_list, make_optional(Type(typeid(Nothing))), std::move(args));
    }
public:
    void step_into_list(shared_ptr<Context> ctx, std::function<void(NodeInContext)> f) const override {
        using List = vector<NodeInContext>;
        using Iter = List::const_iterator;
        auto args = this->get_property("arguments_list");
        if (!args)
            throw NodeAccessError("arguments list is null");
        auto list_of_lists = args->get_list_links(ctx);
        if (list_of_lists.size() == 0)
            return;
        vector<List> links;
        vector<pair<Iter, Iter>> iterators;
        bool fail = false;
        std::transform(
            std::begin(list_of_lists),
            std::end(list_of_lists),
            std::back_inserter(links),
            [&fail](auto e) {
                try {
                    return e.node->get_list_links(e.context);
                } catch (...) {
                }
                fail = true;
                return List();
            }
        );
        if (fail) {
            return;
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
        auto type = get_node_type()->get(ctx);
        while (true) {
            auto node = make_node_with_name<AbstractValue>(type);
            auto list_node = dynamic_cast<AbstractListLinked*>(node.get());
            if (!list_node)
                return;
            size_t i = 0;
            for (auto& e : iterators) {
                if (e.first == e.second)
                    return;
                // TODO: fix contexts
                list_node->set_link(i, e.first->node);
                ++e.first;
                ++i;
            }
            f({node, ctx});
        }
    }
private:
    NODE_PROPERTY(node_type, string);
    NODE_LIST_PROPERTY(arguments_list, Nothing);
};

NODE_INFO_TEMPLATE(DynamicListZip, DynamicListZip<T>, vector<T>);
TYPE_INSTANCES(DynamicListZipNodeInfo)

} // namespace nodes
} // namespace rainynite::core
