/*  fork_pipe.h - fork, exec & setup pipe helper
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

#ifndef CORE_OS_FORK_PIPE_H_427F801D_F129_5166_86AB_120E8310CFF6
#define CORE_OS_FORK_PIPE_H_427F801D_F129_5166_86AB_120E8310CFF6

#include <cstdio>

#include <sys/types.h>

#include <core/std/string.h>
#include <core/std/vector.h>

namespace rainynite {

/**
 * Pipe, fork & exec
 *
 * @return pid of new process
 */
pid_t fork_pipe(FILE*& write_pipe, FILE*& read_pipe, vector<string> args);

} // namespace rainynite

#endif
