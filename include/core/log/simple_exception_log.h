/*
 *  simple_exception_log.h - simple exception log implementation
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

#ifndef CORE_LOG_SIMPLE_EXCEPTION_LOG_H_C836E71D_C128_5309_B73E_639F5266F854
#define CORE_LOG_SIMPLE_EXCEPTION_LOG_H_C836E71D_C128_5309_B73E_639F5266F854

#include <utility>

#include <core/std/queue.h>
#include "exception_log.h"

namespace rainynite::core {

class SimpleExceptionLogger : public ExceptionLogger {
public:
    SimpleExceptionLogger(size_t size_ = 64) :
        size(size_)
    {}

    void log_exception(weak_ptr<ExceptionSource const> source, std::exception const& ex) const noexcept override;

private:
    mutable queue<std::pair<weak_ptr<ExceptionSource const>, std::exception>> log;
    size_t size;
};

} // namespace rainynite::core

#endif
