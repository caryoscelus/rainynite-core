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
    std::cout << "SvgRenderer start" << std::endl;
    context = std::move(context_);
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
    finish_render();
    std::cout << "SvgRenderer done" << std::endl;
    finished = true;
    document.reset();
}

bool SvgRenderer::is_finished() {
    return finished;
}

void SvgRenderer::prepare_render() {
    if (settings.render_pngs)
        start_png();
}

void SvgRenderer::render_frame(Time time) {
    auto path = morph_path(time);
    auto base_name = "renders/{:.3f}"_format(time.get_seconds());
    auto svg_name = base_name+".svg";
    std::cout << svg_name << std::endl;
    std::ofstream f(svg_name);
    fmt::print(f, svg_template, Geom::knots_to_svg(path));
    f.close();
    if (settings.render_pngs)
        render_png(svg_name, base_name+".png");
    finished_frame()(time);
}

void SvgRenderer::finish_render() {
    if (settings.render_pngs)
        quit_png();
}

void SvgRenderer::start_png() {
    png_renderer_pipe = popen("inkscape --shell", "w");
}

void SvgRenderer::render_png(std::string const& svg, std::string const& png) {
    fputs("{} {} {}\n"_format(svg, "-e", png).c_str(), png_renderer_pipe);
}

void SvgRenderer::quit_png() {
    fputs("quit\n", png_renderer_pipe);
    pclose(png_renderer_pipe);
}

Geom::BezierKnots SvgRenderer::morph_path(Time time) {
    auto root = std::dynamic_pointer_cast<BaseValue<Geom::BezierKnots>>(document->get_root());
    return root->get(time);
}

} // namespace filters

} // namespace core
