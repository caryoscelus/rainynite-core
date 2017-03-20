/*
 *  svg_renderer.cpp - primitive .svg renderer
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

#include <cstdlib>

#include <fstream>

#include <fmt/ostream.h>
#include <fmt/format.h>

#include <core/renderers/svg_renderer.h>
#include <core/document.h>

#include <geom_helpers/knots.h>

#include <morphing/morphing.h>

using namespace fmt::literals;

namespace core {

namespace renderers {

const std::string svg_template =
R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg xmlns="http://www.w3.org/2000/svg"
     version="1.1"
     width="800"
     height="600">
  <path id="morph" d="{}" style="fill:black;fill-opacity:1;stroke:none" />
</svg>
)";

void SvgRenderer::render(Context context_) {
    finished = false;
    context = context_;
    document = context.get_document();
    if (!document) {
        throw "can't renderer - no document";
    }
    boost::any maybe_settings = context.get_render_settings();
    if (!maybe_settings.empty()) {
        settings = boost::any_cast<SvgRendererSettings>(maybe_settings);
    }
    prepare_render();
    for (auto time : context.get_period()) {
        render_frame(time);
    }
    finished = true;
    document.reset();
}

bool SvgRenderer::is_finished() {
    return finished;
}

void SvgRenderer::prepare_render() {
    auto count = document->keyframe_amount();
    if (count == 0)
        throw "invalid document";
    auto keyframes = document->get_keyframes();
    if (count == 1) {
        from = keyframes[0];
        to = keyframes[0];
    } else if (count == 2) {
        morphing::prepare_average(keyframes[0], keyframes[1], from, to);
    } else {
        throw "not implemented";
    }
}

void SvgRenderer::render_frame(Time time) {
    auto path = morph_path(time);
    auto base_name = "renders/{:.3f}"_format(time.get_seconds());
    auto svg_name = base_name+".svg";
    std::cout << svg_name << std::endl;
    std::ofstream f(svg_name);
    fmt::print(f, svg_template, Geom::knots_to_svg(path));
    if (settings.render_pngs) {
        auto png_name = base_name+".png";
        system("{} {} {} {}"_format("inkscape -z", svg_name, "-e", png_name).c_str());
    }
}

Geom::BezierKnots SvgRenderer::morph_path(Time time) {
    auto t = time.get_seconds();
    if (t <= 0)
        return from;
    if (t >= 1)
        return to;
    return morphing::simple_average(from, to, t);
}

} // namespace filters

} // namespace core
