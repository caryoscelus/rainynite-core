/*  parse.cpp - time parsing
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

#include <sstream>

#include <core/util/exceptions.h>
#include <core/time/parse.h>

namespace rainynite::core {

TimePeriod parse_time_period(string const& s) {
    std::istringstream stream(s);
    return parse_time_period(stream);
}

TimePeriod parse_time_period(std::istream& stream) {
    if (stream.get() != '[')
        throw ParseError("Unsupported time period format: expected '['");
    auto a = parse_time(stream);
    if (stream.get() != ',')
        throw ParseError("Unsupported time period format: expected ','");
    auto b = parse_time(stream);
    if (stream.get() != ')')
        throw ParseError("Unsupported time period format: expected ')'");
    return TimePeriod(a, b);
}

Time parse_time(string const& s) {
    std::istringstream stream(s);
    return parse_time(stream);
}

Time parse_time(std::istream& stream) {
    Time result;
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

} // namespace rainynite::core
