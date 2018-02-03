/*  serialize/reader.h - document reader interface
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

#ifndef CORE_SERIALIZE_READER_H_DBEA2CAD_1E49_53C1_BF95_1B25F5186F62
#define CORE_SERIALIZE_READER_H_DBEA2CAD_1E49_53C1_BF95_1B25F5186F62

#include <istream>

#include <core/std/memory.h>
#include <core/util/class_init.h>
#include <core/abstract_factory.h>

namespace rainynite::core {

class AbstractDocument;

/**
 * Minimalist document reader interface
 */
class DocumentReader {
public:
    virtual ~DocumentReader() = default;
    virtual shared_ptr<AbstractDocument> read_document(std::istream& input) = 0;

    shared_ptr<AbstractDocument> try_load(std::istream& input) {
        try {
            return read_document(input);
        } catch (std::exception const& ex) {
            // TODO
        } catch (...) {
            // TODO
        }
        return nullptr;
    }
};

#define FILTER_READ(Self) \
    public DocumentReader, \
    private class_init::StringRegistered< \
        Self, \
        DocumentReader \
    >

#define FILTER_NAME(_name) \
public: \
    static string name() { \
        return _name; \
    }

inline map<string, DocumentReader*> const& all_read_filters() {
    return class_init::string_registry<DocumentReader>();
}

} // namespace rainynite::core

#endif
