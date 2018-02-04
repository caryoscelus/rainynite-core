/*  exception_log.h - exception logging classes
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

#ifndef CORE_LOG_EXCEPTION_LOG_H_E986976B_AEA1_5C3F_B5B0_FBD290939F9D
#define CORE_LOG_EXCEPTION_LOG_H_E986976B_AEA1_5C3F_B5B0_FBD290939F9D

#include <exception>

#include <core/std/memory.h>

namespace rainynite::core {

/**
 * Empty interface for all exception sources
 */
class ExceptionSource {
public:
    virtual ~ExceptionSource() = default;
};

/**
 * Exception logger interface
 */
class ExceptionLogger {
public:
    virtual ~ExceptionLogger() = default;
    /// Write exception ex (caused by source) to log
    virtual void log_exception(ExceptionSource const* source, std::exception const& ex) const noexcept = 0;
};

/**
 * Proxy exception logger
 */
class HasExceptionLogger :
    public ExceptionLogger,
    public ExceptionSource
{
public:
    explicit HasExceptionLogger(shared_ptr<ExceptionLogger> logger_ = nullptr) :
        logger(logger_)
    {}

    void set_logger(shared_ptr<ExceptionLogger> logger_) {
        logger = logger_;
    }

    void log_exception_from_this(std::exception const& ex) const {
        log_exception(this, ex);
    }

    void log_exception(ExceptionSource const* source, std::exception const& ex) const noexcept override {
        if (logger)
            logger->log_exception(source, ex);
    }

private:
    shared_ptr<ExceptionLogger> logger;
};

} // namespace rainynite::core

#endif
