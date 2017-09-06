/*
 *  value.h - Node Value
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

#ifndef __CORE__NODE__VALUE_H__B8E31170
#define __CORE__NODE__VALUE_H__B8E31170

#include "abstract_value.h"

namespace rainynite::core {

template <typename T>
class Value : public BaseValue<T> {
public:
    bool is_const() const override {
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

template <typename T, typename... Args>
shared_ptr<Value<T>> make_value_(Args&&... args);

} // namespace rainynite::core

#endif
