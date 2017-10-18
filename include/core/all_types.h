/*  all_types.h - macros for instancing templates with registered types
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

#ifndef CORE_ALL_TYPES_H_4CC9FF4B_9D09_58CD_A7F7_AD5A35DF55EF
#define CORE_ALL_TYPES_H_4CC9FF4B_9D09_58CD_A7F7_AD5A35DF55EF

#include "time/time_period.h"
#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/circle.h>
#include <geom_helpers/knots.h>
#include <2geom/affine.h>
#include <core/color.h>
#include <core/renderable.h>
#include <core/audio.h>
#include <core/time_point.h>
#include <core/nothing.h>

#define TYPE_INSTANCES_WO_RENDERABLE(Template) \
    template class Template<Nothing>; \
    template class Template<bool>; \
    template class Template<double>; \
    template class Template<string>; \
    template class Template<Time>; \
    template class Template<TimePeriod>; \
    template class Template<Geom::Point>; \
    template class Template<Geom::NullShape>; \
    template class Template<Geom::BezierKnots>; \
    template class Template<Geom::Rectangle>; \
    template class Template<Geom::Circle>; \
    template class Template<Geom::Knot>; \
    template class Template<Geom::Affine>; \
    template class Template<colors::Color>;

#define TYPE_INSTANCES(Template) \
    TYPE_INSTANCES_WO_RENDERABLE(Template) \
    template class Template<Renderable>; \
    template class Template<Audio>;

#endif
