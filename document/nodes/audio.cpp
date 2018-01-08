/*  audio.cpp - audio related nodes
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

#include <core/node_info/macros.h>
#include <core/node/node.h>
#include <core/node/property.h>
#include <core/audio.h>

namespace rainynite::core::nodes {

class EmptyAudio : public AudioNode {
    DOC_STRING(
        "Empty audio node"
    )
public:
    EmptyAudio() {
    }
};

REGISTER_NODE(EmptyAudio);

class AudioFromFile : public AudioNode {
    DOC_STRING(
        "Play audio from file.\n"
        "\n"
        "File format support depends on rendering engine."
    )
public:
    AudioFromFile() {
        init<string>(file_path, "");
    }

private:
    NODE_PROPERTY(file_path, string);
};

REGISTER_NODE(AudioFromFile);

} // namespace rainynite::core::nodes
