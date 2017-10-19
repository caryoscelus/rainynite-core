/*  renderable.h - renderable type
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

#ifndef CORE_RENDERABLE_H_0A68F741_3D2C_5985_89F7_AEABEADCA11B
#define CORE_RENDERABLE_H_0A68F741_3D2C_5985_89F7_AEABEADCA11B

#include <core/node/node.h>
#include <core/node/property.h>

namespace rainynite::core {

/**
 * "Tag" type which is used to identify renderable nodes.
 *
 * The idea is that there are renderable nodes, but instead of returning
 * some real render result object they simply hold info that can be used
 * by renderer.
 */
struct Renderable final {
};

/**
 * Base class for all renderable nodes.
 *
 * Since Renderable is only tag class, the point of this base class
 * is to provide empty get() override.
 */
class RenderableNode : public Node<Renderable> {
public:
    Renderable get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
};

} // namespace rainynite::core

#endif
