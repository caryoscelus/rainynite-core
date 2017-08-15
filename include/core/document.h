/*
 *  document.h - document
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

#ifndef __CORE__DOCUMENT_H__62AC426A
#define __CORE__DOCUMENT_H__62AC426A

#include <memory>

#include <geom_helpers/knots.h>

#include <core/time/time_period.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/renderable.h>
#include <core/action.h>

namespace core {

class Context;

class DocumentType {
};

class Document :
    public Node<DocumentType>,
    public std::enable_shared_from_this<Document>
{
public:
    explicit Document(std::shared_ptr<BaseValue<Renderable>> root_=nullptr);
    virtual ~Document();
    std::shared_ptr<Context> get_default_context();
    inline std::shared_ptr<ActionStack> get_action_stack() {
        return action_stack;
    }

public:
    DocumentType get(std::shared_ptr<Context> context) const override;

private:
    std::shared_ptr<Context> default_context;
    std::shared_ptr<ActionStack> action_stack;

    NODE_PROPERTY(root, Renderable);
    NODE_PROPERTY(size, Geom::Point);
    NODE_STATIC_PROPERTY(main_time_period, TimePeriod);
};

}

#endif
