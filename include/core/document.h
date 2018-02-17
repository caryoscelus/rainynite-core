/*  document.h - document
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

#ifndef CORE_DOCUMENT_H_87F22B17_CBE2_5508_B5CD_09A0EBABED26
#define CORE_DOCUMENT_H_87F22B17_CBE2_5508_B5CD_09A0EBABED26

#include <core/std/memory.h>
#include <core/std/string.h>

namespace rainynite::core {

class Context;
class ActionStack;
class NodeTree;

struct DocumentType final {};

class AbstractDocument {
public:
    virtual ~AbstractDocument() {
    }

    virtual shared_ptr<Context> get_default_context() = 0;
    virtual shared_ptr<ActionStack> get_action_stack() = 0;
    virtual shared_ptr<NodeTree> get_tree() = 0;

    string get_path() const;
    void set_path(string const& path_);

private:
    string path;
};

template <class From>
shared_ptr<AbstractDocument> document_cast(From&& pointer) {
    return dynamic_pointer_cast<AbstractDocument>(std::forward<From>(pointer));
}

shared_ptr<AbstractDocument> make_document();

} // namespace rainynite::core

#endif
