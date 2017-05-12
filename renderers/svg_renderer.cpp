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
#include <chrono>
#include <thread>

#include <unistd.h>
#include <fcntl.h>

#include <fstream>

#include <fmt/ostream.h>
#include <fmt/format.h>

#include <core/renderers/svg_renderer.h>
#include <core/document.h>
#include <core/color.h>
#include <core/renderable.h>
#include <core/nodes/composite.h>

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
     xmlns:xlink="http://www.w3.org/1999/xlink"
     version="1.1"
     width="{}px"
     height="{}px"
     viewBox="0 0 {} {}">
  {}
  {}
</svg>
)";

const std::string svg_path = R"(<path d="{}" style="fill:{};fill-opacity:{};stroke:none;{}" />)";

const std::string svg_rectangle = R"(<rect x="{}" y="{}" width="{}" height="{}" style="fill:{};fill-opacity:{};stroke:none;{}"/>)";

const std::string svg_image = R"(<image xlink:href="{}" width="{}" height="{}" x="0" y="0" preserveAspectRatio="none"/>)";

const std::string svg_translate = R"svg(<g transform="translate({}, {})">{}</g>)svg";

const std::string svg_text = R"(<text x="0" y="0" font-size="{}px" fill="{}">{}</text>)";

void SvgRenderer::render(Context context_) {
    finished = false;
    rendered_frames_count = 0;
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
        ++rendered_frames_count;
    }
    finish_render();
    std::cout << "SvgRenderer done" << std::endl;
    finished = true;
}

bool SvgRenderer::is_finished() {
    return finished;
}

void SvgRenderer::prepare_render() {
    if (settings.render_pngs)
        start_png();
}

void SvgRenderer::render_frame(Time time) {
    auto base_name = "renders/{:.3f}"_format(time.get_seconds());
    auto svg_name = base_name+".svg";
    std::cout << svg_name << std::endl;
    std::ofstream f(svg_name);
    auto size = document->get_size()->get(time);
    auto viewport_size = document->get_viewport_size()->get(time);
    auto definitions = document->get_property_value<std::string>("_svg_definitions", time).value_or("");
    fmt::print(f, svg_template, size.x(), size.y(), viewport_size.x(), viewport_size.y(), definitions, frame_to_svg(time));
    f.close();
    if (settings.render_pngs)
        render_png(svg_name, base_name+".png");
    finished_frame()(time);
}

std::string SvgRenderer::frame_to_svg(Time time) const {
    return node_to_svg(document->get_root(), time);
}

std::string get_extra_style(AbstractNode* node, Time time, SvgRendererSettings settings) {
    if (settings.extra_style)
        return node->get_property_value<std::string>("_svg_style", time).value_or("");
    return "";
}

std::string SvgRenderer::node_to_svg(AbstractReference node_ptr, Time time) const {
    if (!node_ptr)
        return "";
    auto node = dynamic_cast<AbstractNode*>(node_ptr.get());
    if (node_ptr->get_type() != typeid(Renderable)) {
        std::cerr << "ERROR: node isn't renderable" << std::endl;
        // throw
        return "";
    }
    // TODO: modularize
    auto name = node_name(*node_ptr);
    // TODO: Animated support
    if (name == "PathShape") {
        auto path = node->get_property_as<Geom::BezierKnots>("path")->get(time);
        auto color = node->get_property_as<colors::Color>("fill_color")->get(time);
        auto extra_style = get_extra_style(node, time, settings);
        return fmt::format(svg_path, Geom::knots_to_svg(path), colors::to_hex24(color), color.alpha(), extra_style);
    } else if (name == "RectangleShape") {
        auto pos = node->get_property_as<Geom::Point>("position")->get(time);
        auto size = node->get_property_as<Geom::Point>("size")->get(time);
        auto color = node->get_property_as<colors::Color>("fill_color")->get(time);
        auto extra_style = get_extra_style(node, time, settings);
        return fmt::format(svg_rectangle, pos.x(), pos.y(), size.x(), size.y(), colors::to_hex24(color), color.alpha(), extra_style);
    } else if (name == "Image") {
        auto fname = node->get_property_as<std::string>("file_path")->get(time);
        auto size = node->get_property_as<Geom::Point>("size")->get(time);
        return fmt::format(svg_image, fname, size.x(), size.y());
    } else if (name == "Translate") {
        auto source = node->get_property("source");
        auto offset = node->get_property_as<Geom::Point>("offset")->get(time);
        return fmt::format(svg_translate, offset.x(), offset.y(), node_to_svg(source, time));
    } else if (name == "Text") {
        auto text = node->get_property_as<std::string>("text")->get(time);
        auto size = node->get_property_as<double>("size")->get(time);
        auto color = node->get_property_as<colors::Color>("color")->get(time);
        return fmt::format(svg_text, size, to_hex24(color), text);
    } else if (auto composite = dynamic_cast<nodes::Composite*>(node)) {
        auto node_list = composite->list_layers()->get_links();
        std::string s;
        for (auto const& node : node_list) {
            s += node_to_svg(node, time);
        }
        return s;
    } else {
        std::cerr << "ERROR: Root node type isn't supported" << std::endl;
        // throw
        return "";
    }
}

void SvgRenderer::finish_render() {
    document.reset();
    if (settings.render_pngs)
        quit_png();
}

void SvgRenderer::start_png() {
    // TODO: wrap all this C mess and move out of here
    int write_pipe_ds[2];
    pipe(write_pipe_ds);
    int read_pipe_ds[2];
    pipe(read_pipe_ds);

    png_renderer_pid = fork();
    if (png_renderer_pid == 0) {
        dup2(write_pipe_ds[0], STDIN_FILENO);
        close(write_pipe_ds[0]);
        close(write_pipe_ds[1]);

        dup2(read_pipe_ds[1], STDOUT_FILENO);
        dup2(read_pipe_ds[1], STDERR_FILENO);
        close(read_pipe_ds[0]);
        close(read_pipe_ds[1]);

        execl("/usr/bin/env", "env", "inkscape", "--shell", (char*)nullptr);
        throw std::runtime_error("execl returned");
    }

    png_renderer_pipe = fdopen(write_pipe_ds[1], "w");
    close(write_pipe_ds[0]);

    fcntl(read_pipe_ds[0], F_SETFL, O_NONBLOCK);
    png_renderer_pipe_output = fdopen(read_pipe_ds[0], "r");
    close(read_pipe_ds[1]);
}

void SvgRenderer::render_png(std::string const& svg, std::string const& png) {
    fputs("{} {} {}\n"_format(svg, "-e", png).c_str(), png_renderer_pipe);
}

void SvgRenderer::quit_png() {
    fputs("quit\n", png_renderer_pipe);
    pclose(png_renderer_pipe);

    // now wait until inkscape renders all the frames..
    auto buff = std::make_unique<char[]>(256);
    std::string sbuff;
    size_t frames_count = 0;
    while (frames_count < rendered_frames_count) {
        size_t read_chars;
        while ((read_chars = fread((void*)buff.get(), 1, 256, png_renderer_pipe_output)) > 0) {
            auto s = std::string(buff.get(), read_chars);
            std::cout << s;
            sbuff += s;
            size_t found;
            while ((found = sbuff.find("Bitmap saved as:")) != std::string::npos) {
                ++frames_count;
                sbuff = sbuff.substr(found+1);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(64));
    }
}

} // namespace filters

} // namespace core
