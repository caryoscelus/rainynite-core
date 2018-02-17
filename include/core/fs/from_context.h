/*  from_context.h - path from context helper
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

#ifndef CORE_FS_FROM_CONTEXT_H_27588489_8731_5596_953B_814FE1E1DB11
#define CORE_FS_FROM_CONTEXT_H_27588489_8731_5596_953B_814FE1E1DB11

#include <core/context.h>
#include <core/document.h>
#include <core/util/nullptr.h>
#include "path.h"

namespace rainynite::core::fs {

Path context_path(Context const& ctx, Path::path_t path) {
    if (path.is_absolute()) {
        return Path{path};
    } else {
        return Path{path, no_null(ctx.get_document())->get_path()};
    }
}

} // namespace rainynite::core::fs

#endif
