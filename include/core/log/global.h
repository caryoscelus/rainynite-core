/*  log/global.h - proxy to a signleton instance of real log
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

#ifndef CORE_LOG_GLOBAL_H_1A2566E7_79AE_5443_87A7_1B5CFF71E9ED
#define CORE_LOG_GLOBAL_H_1A2566E7_79AE_5443_87A7_1B5CFF71E9ED

namespace rainynite::core {

template <class L>
class GlobalLog : public ExceptionLogger {
public:
    static L instance() {
        static L _instance;
        return _instance;
    }

    void log_exception(ExceptionSource const* source, std::exception const& ex) const noexcept override {
        instance().log_exception(source, ex);
    }
};

} // namespace rainynite::core

#endif
