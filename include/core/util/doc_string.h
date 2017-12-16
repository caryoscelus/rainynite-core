/*  doc_string.h - base class for self-documented types
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

#ifndef CORE_UTIL_DOC_STRING_H_8C65081C_80FB_530C_8E03_3E23E09291D2
#define CORE_UTIL_DOC_STRING_H_8C65081C_80FB_530C_8E03_3E23E09291D2

#include <core/std/map.h>
#include <core/std/string.h>

namespace rainynite {

class DocString {
public:
    virtual string doc_string() const {
        return "Undocumented";
    }

    virtual map<string,string> doc_substrings() const {
        return {};
    }

    string doc_substring(string const& sub) const {
        auto mp = doc_substrings();
        auto it = mp.find(sub);
        if (it != mp.end())
            return it->second;
        return "Undocumented";
    }
};

} // namespace rainynite

#define DOC_STRING(s) \
public: \
    string doc_string() const override { \
        return s; \
    }

#endif
