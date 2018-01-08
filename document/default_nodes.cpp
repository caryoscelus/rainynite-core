/*  default_nodes.cpp - default nodes list
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

#include <core/node_info/default_node.h>
#include <core/node/value.h>
#include <core/all_types.h>
#include <core/audio.h>
#include <core/renderable.h>

namespace rainynite::core {

template <typename T>
class DefaultNodeInfoValue : public DefaultNodeInfoImpl<T, Value<T>> {
};

TYPE_INSTANCES_WO_RENDERABLE(DefaultNodeInfoValue)

template struct DefaultNodeInfoImpl<Renderable, Empty>;
template struct DefaultNodeInfoImpl<Audio, EmptyAudio>;

} // namespace rainynite::core
