/*  document.cpp - document
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

#include <core/node/new_node.h>
#include <core/node_info/macros.h>
#include <core/context.h>
#include <core/node_tree/node_tree.h>
#include <core/action_stack.h>
#include <core/time/period.h>
#include <core/renderable.h>
#include <core/audio.h>
#include <core/document.h>

#include <2geom/point.h>

namespace rainynite::core {

class Document :
    public NewNode<
        Document,
        DocumentType,
        types::Only<Renderable>,
        types::Only<Geom::Point>,
        types::Only<TimePeriod>,
        types::Only<Audio>
    >,
    public AbstractDocument
{
    DOC_STRING(
        "Document is the core node of any RainyNite document."
    )

    NODE_PROPERTIES("root", "size", "main_time_period", "soundtrack")
    COMPLEX_DEFAULT_VALUES(
        make_node_with_name("Composite"),
        make_value<Geom::Point>(320, 240),
        make_value<TimePeriod>(Time(0, 12), Time(5, 12)),
        make_node_with_name("AudioFromFile")
    )

public:
    explicit Document(shared_ptr<BaseValue<Renderable>> root_=nullptr);
    virtual ~Document() {
    }

    shared_ptr<Context> get_default_context() override;
    shared_ptr<ActionStack> get_action_stack() override {
        return action_stack;
    }

    DocumentType get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }

    shared_ptr<NodeTree> get_tree() override;

private:
    shared_ptr<Context> default_context;
    shared_ptr<ActionStack> const action_stack;
    shared_ptr<NodeTree> tree;
};

REGISTER_NODE(Document);

Document::Document(shared_ptr<BaseValue<Renderable>> root_) :
    default_context(nullptr),
    action_stack(make_shared<ActionStack>())
{
    if (root_)
        set_property("root", root_);
}

shared_ptr<NodeTree> Document::get_tree() {
    if (tree == nullptr)
        tree = make_shared<NodeTree>(static_pointer_cast<Document>(shared_from_this()));
    return tree;
}

shared_ptr<Context> Document::get_default_context() {
    if (!default_context) {
        default_context = make_shared<Context>(static_pointer_cast<Document>(shared_from_this()));
    }
    return default_context;
}

shared_ptr<AbstractDocument> make_document() {
    return make_shared<Document>();
}

} // namespace rainynite::core
