/*  document_loader.cpp - global document loader & registry
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

#include <fstream>

#include <core/document.h>
#include <core/fs/document_loader.h>

namespace rainynite::core {

shared_ptr<AbstractDocument> DocumentLoader::get_document(fs::Path const& fpath) {
    auto proper_path = fpath.to_absolute();
    auto iter = document_registry.find(proper_path);
    if (iter != document_registry.end())
        return iter->second.document;
    std::ifstream in_file(proper_path);
    return get_document_from(fpath, in_file);
}

shared_ptr<AbstractDocument> DocumentLoader::get_document_from(fs::Path const& fpath, std::istream& in) {
    auto proper_path = fpath.to_absolute();

    for (auto filter_pair : all_read_filters()) {
        auto const& name = filter_pair.first;
        auto filter = filter_pair.second;
        if (auto document = filter->try_load(in)) {
            document->set_path(proper_path);
            document_registry.try_emplace(proper_path, document, name);
            return document;
        }
    }

    return nullptr;
}

string const& DocumentLoader::get_text(fs::Path const& fpath, bool reload) {
    auto proper_path = fpath.to_absolute();
    if (!reload) {
        auto iter = text_registry.find(proper_path);
        if (iter != text_registry.end())
            return iter->second;
    }
    std::ifstream in_file(proper_path);
    std::stringstream buffer;
    buffer << in_file.rdbuf();
    auto [iter, _] = text_registry.try_emplace(proper_path, buffer.str());
    return iter->second;
}

void DocumentLoader::register_document(fs::Path const& fpath, shared_ptr<AbstractDocument> document, string const& format) {
    auto proper_path = fpath.to_absolute();
    document_registry.try_emplace(proper_path, document, format);
}

void DocumentLoader::write_document(fs::Path const& fpath) {
    auto proper_path = fpath.to_absolute();
    std::ofstream of(proper_path);
    write_document_to(fpath, of);
}

void DocumentLoader::write_document_to(fs::Path const& fpath, std::ostream& out) {
    auto proper_path = fpath.to_absolute();
    auto iter = document_registry.find(proper_path);
    if (iter == document_registry.end()) {
        throw std::runtime_error("Cannot find document to write");
    }
    auto [document, filter_name] = iter->second;
    using class_init::name_info;
    auto& filter = name_info<DocumentWriter>(filter_name);
    filter.write_document(out, document);
}

void DocumentLoader::write_recursively(fs::Path const& fpath) {
    write_document(fpath);
    throw std::logic_error("Not implemented");
}

void DocumentLoader::unload_document(fs::Path const& fpath) {
    document_registry.erase(fpath.to_absolute());
}

} // namespace rainynite::core
