/*
 *  document.cpp - document
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

#include <core/document.h>
#include <core/context.h>

namespace core {

Document::Document(std::vector<Geom::BezierKnots> keyframes_) :
    keyframes(keyframes_),
    default_context(nullptr),
    main_time_period(Time(0, 8), Time(5, 8))
{
}

Document::~Document() {
}

std::shared_ptr<Context> Document::get_default_context() {
    if (!default_context) {
        default_context = std::make_shared<Context>(shared_from_this());
    }
    return default_context;
}

} // namespace core
