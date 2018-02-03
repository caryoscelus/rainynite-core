/*  audio_node.h - abstract audio node & EmptyAudio
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

#ifndef CORE_AUDIO_NODE_H_81DB1E03_889B_54B4_9103_4097AAD6DBD1
#define CORE_AUDIO_NODE_H_81DB1E03_889B_54B4_9103_4097AAD6DBD1

#include <core/node/new_node.h>
#include "audio.h"

namespace rainynite::core {

template <class Self, typename... Args>
class AudioNode :
    public NewNode<
        Self,
        Audio,
        Args...
    >
{
protected:
    Audio get(shared_ptr<Context> /*ctx*/) const override {
        return {};
    }
};

class EmptyAudio : public AudioNode<EmptyAudio> {
    DOC_STRING(
        "Empty audio node"
    )

    NODE_PROPERTIES()
    DEFAULT_VALUES()
};

} // namespace rainynite::core

#endif
