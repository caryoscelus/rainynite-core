/*  has_id.h - template for entities with Id
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

#ifndef CORE_NODE_HAS_ID_H_85A247EB_F007_5E20_80F3_8F1BA2F3FB36
#define CORE_NODE_HAS_ID_H_85A247EB_F007_5E20_80F3_8F1BA2F3FB36

#include <core/std/string.h>

namespace rainynite::core {

template <typename Id, typename Generator>
class HasId {
public:
    Id get_id() const {
        return id;
    }

    void set_id(Id id_) {
        id = id_;
    }

    void new_id() {
        id = Generator()();
    }

private:
    Id id;
};

template <typename Id, typename G>
string id_to_string(HasId<Id, G> const& has_id) {
    if constexpr (is_same_v<Id, string>) {
        return has_id.get_id();
    } else {
        return to_string(has_id.get_id());
    }
}

} // namespace rainynite::core

#endif
