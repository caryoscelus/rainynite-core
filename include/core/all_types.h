/*
 *  types.h - runtime primitive type information
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

#ifndef __CORE__ALL_TYPES_H__969ABB32
#define __CORE__ALL_TYPES_H__969ABB32

#include "time/time_period.h"
#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/knots.h>
#include <core/color.h>
#include <core/renderable.h>
#include <core/time_point.h>
#include <core/nothing.h>

#define TYPE_INSTANCES_WO_RENDERABLE(Template) \
    template class Template<Nothing>; \
    template class Template<bool>; \
    template class Template<double>; \
    template class Template<std::string>; \
    template class Template<Time>; \
    template class Template<TimePeriod>; \
    template class Template<Geom::Point>; \
    template class Template<Geom::NullShape>; \
    template class Template<Geom::BezierKnots>; \
    template class Template<Geom::Rectangle>; \
    template class Template<Geom::Knot>; \
    template class Template<colors::Color>;

#define TYPE_INSTANCES(Template) \
    TYPE_INSTANCES_WO_RENDERABLE(Template) \
    template class Template<Renderable>; \
    template class Template<TimePointType>;

#endif
