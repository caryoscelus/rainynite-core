/*  path.h - file path type for RN
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

#ifndef CORE_FS_PATH_H_AFDC024B_CED7_54EE_A37C_1C111CD967D9
#define CORE_FS_PATH_H_AFDC024B_CED7_54EE_A37C_1C111CD967D9

#include <experimental/filesystem>

namespace rainynite::core::fs {

/**
 * Path can be absolute or relative to project.
 */
class Path {
public:
    enum Type {
        ProjectLocal,
        Absolute
    };

    using path_t = std::experimental::filesystem::path;

    Path(path_t absolute_path) :
        type(Absolute),
        value(absolute_path),
        owner()
    {
//         if (!value.is_absolute())
//             throw std::logic_error("Cannot construct absolute path if it's not absolute..");
    }

    Path(path_t relative, path_t owner_) :
        type(ProjectLocal),
        value(relative),
        owner(owner_)
    {}

    path_t to_absolute() const {
        switch (type) {
            case ProjectLocal:
                return owner / value;
            case Absolute:
                return value;
            default:
                throw std::logic_error("Invalid Type value");
        }
    }

private:
    Type type;
    path_t value;
    path_t owner;
};

} // namespace rainynite::core::fs

#endif
