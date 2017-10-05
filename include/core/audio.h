/*  audio.h - abstract audio type
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

#ifndef CORE_AUDIO_H_6CA600AC_1164_59FE_970D_71C6FFA80A22
#define CORE_AUDIO_H_6CA600AC_1164_59FE_970D_71C6FFA80A22

#include <core/node/node.h>

namespace rainynite::core {

/**
 * Abstract audio type.
 */
struct Audio {
};

class AudioNode : public Node<Audio> {
public:
    Audio get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
};

} // namespace rainynite::core

#endif
