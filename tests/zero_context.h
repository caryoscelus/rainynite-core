/*  zero_context.h - zero context helper for tests
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

#ifndef CORE_TESTS_ZERO_CONTEXT_H_3CDF8C89_6812_56CC_801D_C11BAEDD432C
#define CORE_TESTS_ZERO_CONTEXT_H_3CDF8C89_6812_56CC_801D_C11BAEDD432C

#include <core/context.h>

namespace rainynite::core {

shared_ptr<Context> zero_context() {
    static auto instance = make_shared<Context>();
    return instance;
}

} // namespace rainynite::core

#endif
