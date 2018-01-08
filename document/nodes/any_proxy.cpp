/*  any_proxy.cpp - new kind of all-type node experiment
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

#include <core/node_info/macros.h>
#include <core/node/named_link_storage.h>
#include <core/util/type_info.h>

namespace rainynite::core::nodes {

class AnyProxy :
    public DocString,
    public AbstractValue,
    public NamedLinkStorage<
        AnyProxy,
        types::Any
    >
{
    DOC_STRING(
        "Hold any other node"
    )

    NODE_PROPERTIES("source")
    DEFAULT_VALUES(Nothing{})

    PROPERTY(source)

public:
    bool can_set_source(shared_ptr<AbstractValue> src) const override {
        return src != nullptr;
    }
    void set_source(shared_ptr<AbstractValue> src) override {
        set_link(0, src);
    }

    any get_any(shared_ptr<Context> ctx) const noexcept override {
        if (auto node = p_source())
            return node->get_any(ctx);
        return Nothing{};
    }

    Type get_type() const override {
        if (auto node = p_source())
            return node->get_type();
        return typeid(Nothing);
    }
};

class AnyProxyNodeInfo : public NodeInfo {
public:
    AnyProxyNodeInfo(Type node_type_) :
        node_type(node_type_)
    {}

    string name() const override {
        return "AnyProxy";
    }
    AbstractReference new_empty() const override {
        return make_shared<AnyProxy>();
    }
    AbstractReference clone(AbstractValue const& source) const override {
        return make_shared<AnyProxy>(static_cast<AnyProxy const&>(source));
    }
    Type type() const override {
        return node_type;
    }

private:
    Type node_type;
};

struct AnyProxyTypedInfo : class_init::Initialized<AnyProxyTypedInfo> {
    static void init() {
        auto node_info = new AnyProxyNodeInfo(typeid(Nothing));
        class_init::class_registry<NodeInfo>()[typeid(AnyProxy)] = node_info;
        class_init::reverse_class_registry<string>().emplace(
            "AnyProxy",
            typeid(AnyProxy)
        );
        for (auto const& e : all_types()) {
            node_info = new AnyProxyNodeInfo(e.first);
            node_types()[e.first].insert(node_info);
        }
    }
};

} // namespace rainynite::core::nodes
