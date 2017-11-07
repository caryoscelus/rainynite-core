/*  node_tester.cpp - load .rnite file and test it
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

#include <fstream>

#include <boost/uuid/uuid_io.hpp>

#include <fmt/format.h>

#include <core/std/string.h>
#include <core/filters/yaml_reader.h>
#include <core/document.h>
#include <core/node_info.h>
#include "zero_context.h"

using namespace fmt::literals;

namespace rainynite::core {

bool load_and_test_file(string const& fname) {
    std::ifstream in(fname);
    using filters::YamlReader;
    auto document = YamlReader().read_document(in);
    if (document == nullptr)
        throw std::runtime_error("Unknown parse failure");
    in.close();
    bool failed = false;
    if (auto test_list = document->get_property("_tests")) {
        int i = 0;
        for (auto nic : test_list->list_links(zero_context())) {
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
                failed = true;
            }
            ++i;
        }
    }
    return !failed;
}

} // namespace rainynite::core

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    return rainynite::core::load_and_test_file(argv[1]) ? 0 : 2;
}
