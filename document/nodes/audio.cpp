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
#include <core/audio_node.h>

namespace rainynite::core::nodes {

class AudioFromFile : public AudioNode<AudioFromFile, types::Only<string>> {
    DOC_STRING(
        "Play audio from file.\n"
        "\n"
        "File format support depends on rendering engine."
    )

    NODE_PROPERTIES("file_path")
    DEFAULT_VALUES(string{})
    PROPERTY(file_path)
};

REGISTER_NODE(AudioFromFile);

} // namespace rainynite::core::nodes
