/*  util/crtp.h - curiously recurring template pattern
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

#ifndef UTIL_CRTP_H_B528AAD5_987E_556C_A9AC_F525F36380DE
#define UTIL_CRTP_H_B528AAD5_987E_556C_A9AC_F525F36380DE

namespace crtp {

template <class Base, class Self>
class Crtp {
protected:
    Self* self() {
        assert_crtp();
        return static_cast<Self*>(this);
    }

    Self const* self() const {
        assert_crtp();
        return static_cast<Self const*>(this);
    }

    void assert_crtp() const {
        static_assert(std::is_base_of_v<Crtp<Base, Self>, Base>);
        static_assert(std::is_base_of_v<Base, Self>);
    }
};

} // namespace crtp

#endif
