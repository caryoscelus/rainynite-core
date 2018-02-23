/*  all_types.h - macros for instancing templates with registered types
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

#ifndef CORE_ALL_TYPES_H_4CC9FF4B_9D09_58CD_A7F7_AD5A35DF55EF
#define CORE_ALL_TYPES_H_4CC9FF4B_9D09_58CD_A7F7_AD5A35DF55EF

#include <core/time/period.h>
#include <geom_helpers/null_shape.h>
#include <geom_helpers/rectangle.h>
#include <geom_helpers/circle.h>
#include <geom_helpers/knots.h>
#include <geom_helpers/bones.h>
#include <2geom/affine.h>
#include <core/fs/path.h>
#include <core/node_tree/path.h>
#include <core/shading.h>
#include <core/renderable.h>
#include <core/audio.h>
#include <core/time_point.h>
#include <core/util/nothing.h>

#define TYPE_INSTANCES_WO_RENDERABLE_AND_CUSTOM_IO(Template) \
    template struct Template<Nothing>; \
    template struct Template<string>; \
    template struct Template<core::Time>; \
    template struct Template<core::TimePeriod>; \
    template struct Template<Geom::Point>; \
    template struct Template<Geom::bones::Bone>; \
    template struct Template<Geom::NullShape>; \
    template struct Template<Geom::BezierKnots>; \
    template struct Template<Geom::Rectangle>; \
    template struct Template<Geom::Circle>; \
    template struct Template<Geom::Knot>; \
    template struct Template<core::colors::Color>; \
    template struct Template<core::Shading>; \
    template struct Template<core::NodeTreePath>;

#define TYPE_INSTANCES_WO_RENDERABLE(Template) \
    TYPE_INSTANCES_WO_RENDERABLE_AND_CUSTOM_IO(Template) \
    template struct Template<bool>; \
    template struct Template<double>; \
    template struct Template<Geom::Affine>; \
    template struct Template<core::fs::Path::path_t>;

#define TYPE_INSTANCES(Template) \
    TYPE_INSTANCES_WO_RENDERABLE(Template) \
    template struct Template<core::Renderable>; \
    template struct Template<core::Audio>;

#define NODE_INFO_INSTANCES(Name, Node, NodeType) \
    NODE_INFO_TEMPLATE(Name, Node, NodeType); \
    TYPE_INSTANCES(Name##NodeInfo)

#endif
