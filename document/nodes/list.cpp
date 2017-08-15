/*
 *  list.cpp - list utils
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
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/all_types.h>

namespace core {

class ToUntypedList : public Node<Nothing> {
public:
    ToUntypedList() {
        init_property("source", boost::none, make_value<Nothing>());
    }
public:
    std::vector<NodeInContext> get_list_links(std::shared_ptr<Context> ctx) const override {
        if (auto list = std::dynamic_pointer_cast<AbstractListLinked>(get_property("source")))
            return list->get_list_links(ctx);
        return {};
    }
    Nothing get(std::shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
};

REGISTER_NODE(ToUntypedList);

template <typename T>
class ListElement : public ProxyNode<T> {
public:
    ListElement() {
        this->init_property("source", boost::none, make_value<Nothing>());
        this->template init<double>(n, 0);
    }
    void step_into(std::shared_ptr<Context> ctx, std::function<void(NodeInContext)> f) const override {
        size_t n = std::max(get_n()->get(ctx), 0.0);
        size_t i = 0;
        auto list = this->get_property("source");
        NodeInContext result;
        list->step_into_list(
            ctx,
            [n, &i, &result](NodeInContext nic) {
                if (i == n) {
                    result = nic;
                }
                ++i;
            }
        );
        if (result)
            f(result);
    }
private:
    NODE_PROPERTY(n, double);
};

NODE_INFO_TEMPLATE(ListElement, ListElement<T>, T);
TYPE_INSTANCES(ListElementNodeInfo)

} // namespace core
