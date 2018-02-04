/*  node_tester.cpp - load .rnite file and test it
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

#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/uuid/uuid_io.hpp>

#include <fmt/format.h>

#include <core/std/string.h>
#include <core/document.h>
#include <core/fs/document_loader.h>
#include <core/node/abstract_node.h>
#include <core/node_info/node_info.h>
#include <core/util/nullptr.h>
#include "zero_context.h"

using namespace fmt::literals;

namespace rainynite::core {

bool test_document(shared_ptr<AbstractDocument> document) {
    bool failed = false;
    if (auto test_list = abstract_node_cast(document)->get_property("_tests")) {
        int i = 0;
        for (auto nic : test_list->list_links(document->get_default_context())) {
            if (auto node = dynamic_cast<BaseValue<bool>*>(nic.node.get())) {
                if (node->value(nic.context) != true) {
                    failed = true;
                    std::cerr << "Test #{} failed:\n  id = {}\n  type = {}\n"_format(
                        i,
                        to_string(node->get_id()),
                        node_name(*node)
                    );
                }
            } else {
                std::cerr << "Test #{} failed:\n  test node is not bool";
                failed = true;
            }
            ++i;
        }
    }
    return !failed;
}

bool load_and_test_file(string const& fname) {
    auto loader = DocumentLoader::instance();
    auto path = fs::Path(fname);

    auto document = loader->get_document(path);
    if (document == nullptr)
        throw std::runtime_error("Unknown parse failure");

    if (!test_document(document))
        return false;

    std::cerr << "All tests ok, ";

    std::stringstream saved_content;
    loader->write_document_to(path, saved_content);

    // TODO: avoid extra file read?..
    std::stringstream content;
    content << std::ifstream(fname).rdbuf();

    if (content.str() == saved_content.str()) {
        std::cerr << "saved content identical to source.\n";
        return true;
    }

    std::cerr << "but saved content differs from source\n";
    std::cerr << saved_content.str() << "\n\n";

    if (abstract_node_cast(document)->get_property_value<bool>("_test_save_verbatim", zero_context()).value_or(false)) {
        std::cerr << "This file is supposed to be saved verbatim => test failure.";
        return false;
    }

    std::cerr << "Will try to check if saved version is semantically identical...\n";

    loader->unload_document(path);
    auto saved_document = no_null(loader->get_document_from(path, saved_content));

    return test_document(saved_document);
}

} // namespace rainynite::core

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    return rainynite::core::load_and_test_file(argv[1]) ? 0 : 2;
}
