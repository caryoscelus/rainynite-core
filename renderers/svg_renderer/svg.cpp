/*  svg.cpp - SvgRenderer svg embed
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
#include <regex>

#include <core/std/string.h>

#include "svg_module.h"

namespace rainynite::core::renderers {

class EmbedSvgRenderer : SVG_RENDERER_MODULE_CLASS(EmbedSvgRenderer) {
    SVG_RENDERER_MODULE_NAME("EmbedSvg");
public:
    string operator()(AbstractNode const& node, shared_ptr<Context> ctx, SvgRendererSettings const& /*settings*/) const override {
        auto fname = node.get_property_as<string>("file_path")->value(ctx);
        std::ifstream stream(fname);

        if (!stream) {
            std::cerr << "file '" << fname << "' not found" << std::endl;
            return "";
        }

        string buff;

        {
            std::stringstream buffer;
            buffer << stream.rdbuf();
            stream.close();
            buff = buffer.str();
        }

        std::stringstream result;

        std::regex xml_svg_re(R"(<(\?xml.*\?|svg[^>]*|/svg.*)>)");
        std::regex_replace(
            std::ostreambuf_iterator(result),
            buff.begin(),
            buff.end(),
            xml_svg_re,
            ""
        );

        return result.str();
    }
};

} // namespace rainynite::core::renderers
