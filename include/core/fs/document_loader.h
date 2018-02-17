/*  document_loader.h - global document loader & registry
 *  Copyright (C) 2018 caryoscelus
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

#ifndef CORE_DOCUMENT_LOADER_H_BDC4CC54_00DA_58E6_8177_93FF7C80F1B8
#define CORE_DOCUMENT_LOADER_H_BDC4CC54_00DA_58E6_8177_93FF7C80F1B8

#include <core/std/memory.h>
#include <core/std/map.h>
#include <core/filters/reader.h>
#include <core/filters/writer.h>
#include "path.h"

namespace rainynite::core {

class AbstractDocument;

/**
 * Document & generally file registry.
 */
class DocumentLoader {
public:
    static observer_ptr<DocumentLoader> instance() {
        static auto _instance = make_unique<DocumentLoader>();
        return observer_ptr<DocumentLoader>{_instance.get()};
    }

    /// Get (possibly loaded) document from path
    shared_ptr<AbstractDocument> get_document(fs::Path const& fpath);
    shared_ptr<AbstractDocument> get_document_from(fs::Path const& fpath, std::istream& in);

    /**
     * Get (possibly cached) file contents.
     *
     * TODO: enable proper caching.
     */
    string const& get_text(fs::Path const& fpath, bool reload=true);

    void register_document(fs::Path const& fpath, shared_ptr<AbstractDocument> document, string const& format);

    void write_document(fs::Path const& fpath);
    void write_document_to(fs::Path const& fpath, std::ostream& out);

    void write_recursively(fs::Path const& fpath);

    void unload_document(fs::Path const& fpath);

private:
    struct Document {
        shared_ptr<AbstractDocument> document;
        string filter;

        Document(shared_ptr<AbstractDocument> document_, string filter_) :
            document(document_),
            filter(filter_)
        {}
    };

    map<fs::Path::path_t, Document> document_registry;
    map<fs::Path::path_t, string> text_registry;
};

} // namespace rainynite::core

#endif
