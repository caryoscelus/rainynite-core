/*  list.cpp - list utils
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

#include <core/std/algorithm.h>
#include <core/util/crtp.h>
#include <core/node_info/macros.h>
#include <core/node/proxy_node.h>
#include <core/node/property.h>
#include <core/node/new_node.h>
#include <core/all_types.h>

namespace rainynite::core {

class ToUntypedList : public Node<Nothing> {
public:
    ToUntypedList() {
        init_property("source", {}, make_value<Nothing>());
    }

    Nothing get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }

    size_t list_links_count(shared_ptr<Context> ctx) const override {
        if (auto list = this->get_property("source"))
            return list->list_links_count(ctx);
        return 0;
    }

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        if (auto list = get_property("source"))
            return list->list_links(ctx);
        return {};
    }
};

REGISTER_NODE(ToUntypedList);


/**
 * Convert UntypedListValue to typed list.
 *
 * NOTE: currently doesn't do any type-checking!
 */
template <typename T>
class ToTypedList : public Node<vector<T>> {
public:
    ToTypedList() {
        auto src = make_shared<UntypedListValue>();
        src->new_id();
        this->template init_property("source", Type(typeid(Nothing)), std::move(src));
    }

    size_t list_links_count(shared_ptr<Context> ctx) const override {
        if (auto list = this->get_property("source"))
            return list->list_links_count(ctx);
        return 0;
    }

protected:
    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        if (auto list = this->get_property("source"))
            return list->list_links(ctx);
        return {};
    }
};

NODE_INFO_TEMPLATE(ToTypedList, ToTypedList<T>, vector<T>);
TYPE_INSTANCES(ToTypedListNodeInfo)


template <typename T>
class ListElement : public ProxyNode<T> {
public:
    ListElement() {
        this->init_property("source", {}, make_value<Nothing>());
        this->template init<double>(n, 0);
    }
    NodeInContext get_proxy(shared_ptr<Context> ctx) const override {
        auto list = this->get_property("source");
        auto l = list->list_links(ctx);
        if (l.size() == 0)
            throw NodeAccessError("Requested element of empty list");
        size_t n = clamp(get_n()->value(ctx), 0.0, l.size()-1.0);
        return l[n];
    }
private:
    NODE_PROPERTY(n, double);
};

NODE_INFO_TEMPLATE(ListElement, ListElement<T>, T);
TYPE_INSTANCES(ListElementNodeInfo)


class ListLinkCount :
    public NewNode<
        ListLinkCount,
        double,
        types::Any
    >
{
    DOC_STRING(
        "Return number of list elements in its child\n"
        "\n"
        "Returns 0 for non-lists"
    )

    NODE_PROPERTIES("source")
    DEFAULT_VALUES(Nothing{})

    PROPERTY(source)

protected:
    double get(shared_ptr<Context> ctx) const override {
        return p_source()->list_links_count(ctx);
    }
};

REGISTER_NODE(ListLinkCount);


/**
 * Helper class for numeric sequence generator nodes
 */
template <class Self, class Base>
class AbstractNumericSequence :
    public crtp::Crtp<
        AbstractNumericSequence<Self,Base>,
        Self
    >,
    public Base
{
protected:
     vector<double> get(shared_ptr<Context> ctx) const override {
        vector<double> result;
        this->self()->do_the_job(ctx, [ctx, &result](auto n) {
            result.push_back(n);
        });
        return result;
    }

    vector<NodeInContext> get_list_links(shared_ptr<Context> ctx) const override {
        vector<NodeInContext> result;
        this->self()->do_the_job(ctx, [ctx, &result](auto n) {
            result.emplace_back(make_value<double>(n), ctx);
        });
        return result;
    }

public:
    size_t list_links_count(shared_ptr<Context> ctx) const noexcept override {
        size_t result = 0;
        this->self()->do_the_job(ctx, [&result](auto) {
            ++result;
        });
        return result;
    }
};


class NumericSequence :
    public AbstractNumericSequence<
        NumericSequence,
        NewNode<
            NumericSequence,
            vector<double>,
            types::Only<double>,
            types::Only<double>,
            types::Only<double>
        >
    >
{
    DOC_STRING(
        "Generate number sequence"
    )

    NODE_PROPERTIES("first", "count", "step")
    DEFAULT_VALUES(0.0, 0.0, 1.0)

    PROPERTY(first)
    PROPERTY(count)
    PROPERTY(step)

public:
    template <typename F>
    void do_the_job(shared_ptr<Context> ctx, F f) const {
        auto first = first_value<double>(ctx);
        auto count = count_value<double>(ctx);
        auto step = step_value<double>(ctx);

        for (double i = 0; i < count; ++i) {
            f(first+i*step);
        }
    }

    size_t list_links_count(shared_ptr<Context> ctx) const noexcept override {
        return (size_t)std::max(count_value<double>(ctx), 0.0);
    }
};

REGISTER_NODE(NumericSequence);


class NumericSplitSequence :
    public AbstractNumericSequence<
        NumericSplitSequence,
        NewNode<
            NumericSplitSequence,
            vector<double>,
            types::Only<double>,
            types::Only<double>,
            types::Only<double>
        >
    >
{
    DOC_STRING(
        "Generate number sequence"
    )

    NODE_PROPERTIES("first", "last", "count")
    DEFAULT_VALUES(0.0, 0.0, 1.0)

    PROPERTY(first)
    PROPERTY(last)
    PROPERTY(count)

public:
    template <typename F>
    void do_the_job(shared_ptr<Context> ctx, F f) const {
        auto first = first_value<double>(ctx);
        auto last = last_value<double>(ctx);
        auto count = count_value<double>(ctx);

        for (double i = 0; i < count; ++i) {
            f(first + (last-first)*i/count);
        }
    }

    size_t list_links_count(shared_ptr<Context> ctx) const noexcept override {
        return (size_t)std::max(count_value<double>(ctx), 0.0);
    }
};

REGISTER_NODE(NumericSplitSequence);


class NumericPeriodSequence :
    public AbstractNumericSequence<
        NumericPeriodSequence,
        NewNode<
            NumericPeriodSequence,
            vector<double>,
            types::Only<double>,
            types::Only<double>,
            types::Only<double>
        >
    >
{
    DOC_STRING(
        "Generate number sequence"
    )

    NODE_PROPERTIES("first", "last", "step")
    DEFAULT_VALUES(0.0, 0.0, 1.0)

    PROPERTY(first)
    PROPERTY(last)
    PROPERTY(step)

public:
    template <typename F>
    void do_the_job(shared_ptr<Context> ctx, F f) const {
        auto first = first_value<double>(ctx);
        auto last = last_value<double>(ctx);
        auto step = step_value<double>(ctx);

        if (step == 0 || std::signbit(last-first) != std::signbit(step))
            return;

        auto in_range = [first, last](auto n) {
            if (last > first)
                return first <= n && n < last;
            else
                return first >= n && n > last;
        };

        for (double i = first; in_range(i); i += step) {
            f(i);
        }
    }

    size_t list_links_count(shared_ptr<Context> ctx) const noexcept override {
        auto step = step_value<double>(ctx);
        if (step == 0)
            return 0;
        auto first = first_value<double>(ctx);
        auto last = last_value<double>(ctx);
        return (size_t)std::max((last-first)/step, 0.0);
    }
};

REGISTER_NODE(NumericPeriodSequence);


class AllOf :
    public NewNode<
        AllOf,
        bool,
        types::Only<vector<bool>>
    >
{
    DOC_STRING(
        "Returns whether all of its children are true\n"
        "\n"
        "If any of children is not boolean or invalid node, returns false"
    )

    NODE_PROPERTIES("source")
    COMPLEX_DEFAULT_VALUES(make_node<ListValue<bool>>())
    PROPERTY(source)

protected:
    bool get(shared_ptr<Context> ctx) const override {
        for (auto nic : p_source()->list_links(ctx)) {
            if (!nic || !nic.value_as<bool>())
                return false;
        }
        return true;
    }
};

REGISTER_NODE(AllOf);


} // namespace rainynite::core
