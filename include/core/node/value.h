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

namespace core {

template <typename T>
class Value : public BaseValue<T> {
public:
    virtual bool is_const() const override {
        return true;
    }
    virtual T get(Time) const override {
        return value;
    }
    virtual void set(T value_) override {
        value = value_;
    }
    virtual T& mod() override {
        return value;
    }
    virtual bool can_set() const override {
        return true;
    }
    virtual boost::any any() const override {
        return value;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        value = T(std::forward<Args>(args)...);
    }

private:
    T value;
};

template <typename T, typename... Args>
std::shared_ptr<Value<T>> make_value(Args&&... args);

} // namespace core

#endif
