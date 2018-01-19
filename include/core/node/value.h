/*  value.h - Node Value
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

#ifndef CORE_NODE_VALUE_H_7884E691_994C_52B4_A222_E536C3B5F9D9
#define CORE_NODE_VALUE_H_7884E691_994C_52B4_A222_E536C3B5F9D9

#include "abstract_value.h"

namespace rainynite::core {

template <typename T>
class Value : public BaseValue<T> {
public:
    bool is_static() const override {
        return true;
    }
    T get(shared_ptr<Context> /*context*/) const override {
        return value_;
    }
    void set(T new_value) override {
        value_ = new_value;
        this->changed();
    }
    T& mod() override {
        return value_;
    }
    bool can_set() const override {
        return true;
    }
    any static_any() const override {
        return value_;
    }

public:
    template <typename... Args>
    void emplace(Args&&... args) {
        value_ = T(std::forward<Args>(args)...);
    }

private:
    T value_;
};

} // namespace rainynite::core

#endif
