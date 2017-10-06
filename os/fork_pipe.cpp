/*  fork_pipe.cpp - fork, exec & setup pipe helper
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

#include <stdexcept>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include <core/os/fork_pipe.h>

namespace rainynite {

pid_t fork_pipe(FILE*& write_pipe, FILE*& read_pipe, vector<string> args) {
    if (args.size() < 1)
        throw std::invalid_argument("Empty argument list");

    int write_pipe_ds[2];
    pipe(write_pipe_ds);
    int read_pipe_ds[2];
    pipe(read_pipe_ds);

    auto pid = fork();
    if (pid == 0) {
        dup2(write_pipe_ds[0], STDIN_FILENO);
        close(write_pipe_ds[0]);
        close(write_pipe_ds[1]);

        dup2(read_pipe_ds[1], STDOUT_FILENO);
        dup2(read_pipe_ds[1], STDERR_FILENO);
        close(read_pipe_ds[0]);
        close(read_pipe_ds[1]);

        char const** c_args = new char const*[args.size()+1];
        std::transform(
            std::begin(args),
            std::end(args),
            c_args,
            [](auto const& s) {
                return s.c_str();
            }
        );
        c_args[args.size()] = nullptr;
        execv(args[0].c_str(), const_cast<char* const*>(c_args));
        throw std::runtime_error("execl returned");
    }

    write_pipe = fdopen(write_pipe_ds[1], "w");
    close(write_pipe_ds[0]);

    fcntl(read_pipe_ds[0], F_SETFL, O_NONBLOCK);
    read_pipe = fdopen(read_pipe_ds[0], "r");
    close(read_pipe_ds[1]);

    return pid;
}

} // namespace rainynite
