/*
 *  property.h - Node property macros
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

#ifndef __CORE__NODE__PROPERTY_H__7328135E
#define __CORE__NODE__PROPERTY_H__7328135E

/*
 * NOTE: static_pointer_cast might be unsafe (silently ignoring when node is
 * being used incorrectly), so using dynamic_pointer_cast (which would lead
 * to instant segfault instead) for now. However, it might inflict too much
 * performance hit since getter can be used very frequently.
 * Because of this, we might revert to static or even reinterpret cast in
 * the future...
 */
#define NODE_PROPERTY(name, type) \
public: \
    inline BaseReference<type> get_##name() const { \
        return this->template get_property_as<type>(#name); \
    } \
    inline void set_##name(BaseReference<type> value) { \
        this->set_property(#name, std::dynamic_pointer_cast<AbstractValue>(value)); \
    } \
private: \
    string name { #name }

#define NODE_STATIC_PROPERTY(name, type) \
public: \
    inline shared_ptr<Value<type>> get_##name() const { \
        return std::dynamic_pointer_cast<Value<type>>(this->get_property(#name)); \
    } \
    inline void set_##name(shared_ptr<Value<type>> value) { \
        this->set_property(#name, std::dynamic_pointer_cast<AbstractValue>(value)); \
    } \
private: \
    string name { #name }

#define NODE_LIST_PROPERTY(name, type) \
public: \
    inline AbstractListReference list_##name() const { \
        return std::dynamic_pointer_cast<AbstractListLinked>(this->get_property(#name)); \
    } \
NODE_PROPERTY(name, vector<type>)

#endif
