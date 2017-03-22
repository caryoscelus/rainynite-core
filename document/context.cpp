/*
 *  context.cpp - Context
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

#include <core/context.h>
#include <core/document.h>

namespace core {

Context::Context(std::weak_ptr<Document> document_) :
    document(document_)
{
    auto doc = get_document();
    if (!doc)
        return;
    time_period = doc->get_main_time_period();
}

Context::Context(Context const& context_) :
    Context(context_.document)
{
    render_settings = context_.render_settings;
}

Context::~Context() {
}

} // namespace core
