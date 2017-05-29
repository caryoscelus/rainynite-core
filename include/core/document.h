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

public:
    virtual DocumentType get(Time) const override;

private:
    std::shared_ptr<Context> default_context;

    NODE_PROPERTY(root, Renderable);
    NODE_PROPERTY(size, Geom::Point);
    NODE_PROPERTY(viewport_size, Geom::Point);
    NODE_STATIC_PROPERTY(main_time_period, TimePeriod);
};

}

#endif
