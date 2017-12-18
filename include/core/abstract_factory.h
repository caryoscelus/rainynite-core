/*  abstract_factory.h - Abstract Factory interface template
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

#ifndef CORE_ABSTRACT_FACTORY_H_302C0197_C085_5571_A533_C4B637493FFB
#define CORE_ABSTRACT_FACTORY_H_302C0197_C085_5571_A533_C4B637493FFB

#include <core/std/memory.h>

namespace rainynite {

template <class A>
struct AbstractFactory {
    virtual unique_ptr<A> operator()() const = 0;
};

template <class A, class C>
struct AbstractFactoryImpl : public AbstractFactory<A> {
    unique_ptr<A> operator()() const override {
        return make_unique<C>();
    }
};

} // namespace rainynite

#endif
