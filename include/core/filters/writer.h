/*  writer.h - document writer interface
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

#ifndef CORE_WRITER_H_1930A317_8931_59EA_BFA9_58717ED304F7
#define CORE_WRITER_H_1930A317_8931_59EA_BFA9_58717ED304F7

#include <core/std/memory.h>
#include <ostream>

namespace rainynite::core {

class AbstractDocument;

/**
 * Minimal document writer/serializer interface.
 */
class DocumentWriter {
public:
    virtual void write_document(std::ostream& output, shared_ptr<AbstractDocument> document) = 0;
};

} // namespace rainynite::core

#endif
