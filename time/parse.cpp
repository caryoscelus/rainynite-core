/*
 *  parse.cpp - time parsing
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

#include <sstream>

#include <core/parse.h>
#include <core/time/parse.h>

namespace core {

TimePeriod parse_time_period(std::string const& s) {

}

Time parse_time(std::string const& s) {
    Time result;
    std::istringstream stream(s);
    double frames;
    if (stream >> frames) {
        if (stream.get() == 'f' && stream.get() == '@') {
            int fps;
            if (stream >> fps) {
                result.set_fps(fps);
                result.set_frames(frames);
                return result;
            }
        }
    } else {
        //
    }
    throw ParseError("Unsupported time format");
}

} // namespace core
