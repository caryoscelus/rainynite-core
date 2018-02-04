/*  simple_exception_log.cpp - simple exception log implementation
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

#include <iostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <core/log/simple_exception_log.h>

using namespace fmt::literals;

namespace rainynite::core {

void SimpleExceptionLogger::log_exception(ExceptionSource const* source, std::exception const& ex) const noexcept {
    while (log.size() >= size)
        log.pop();
    log.emplace(source, ex);
    // TODO: notify
    // TODO: flexible output
    std::cerr << "Exception in {}:\n{}\n"_format((void*)source, ex.what());
}

} // namespace rainynite::core
