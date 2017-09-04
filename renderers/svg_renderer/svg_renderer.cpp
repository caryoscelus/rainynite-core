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
#include <fstream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <boost/filesystem.hpp>

#include <fmt/ostream.h>
#include <fmt/format.h>

#include <core/renderers/svg_renderer.h>
#include <core/document.h>
#include <core/color.h>
#include <core/renderable.h>
#include <core/node_info.h>
#include <core/node/proxy_node.h>
#include <core/class_init.h>
#include "svg_module.h"

#include <geom_helpers/knots.h>

#include <morphing/morphing.h>

using namespace fmt::literals;

namespace rainynite::core {

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

struct SvgRenderer::Impl {
    Impl(SvgRenderer* parent_) :
        parent(parent_)
    {}

    void render(Context&& context_);
    void prepare_render();
    void render_frame(std::shared_ptr<Context> context);
    void finish_render();
//     std::string definitions(std::shared_ptr<Context> context) const;
    std::string frame_to_svg(std::shared_ptr<Context> context) const;
    std::string node_to_svg(NodeInContext nic) const;
    void start_png(bool force=false);
    void restart_png() {
        start_png(true);
    }
    void render_png(std::string const& svg, std::string const& png);
    void quit_png(bool force=false);

    bool finished = false;
    Context context;
    std::shared_ptr<Document> document;

    SvgRendererSettings settings;

    boost::filesystem::path render_path { "renders/" };
    boost::filesystem::path base_path;

    FILE* png_renderer_pipe;
    FILE* png_renderer_pipe_output;
    pid_t png_renderer_pid;
    size_t rendered_frames_count;
    bool subprocess_initialized = false;
    bool requested_to_stop = false;
    bool svgs_finished = false;

    std::thread read_thread;

    SvgRenderer* parent;
};

SvgRenderer::SvgRenderer() :
    impl(std::make_unique<Impl>(this))
{
}

SvgRenderer::~SvgRenderer() {
}

void SvgRenderer::render(Context&& context_) {
    impl->render(std::move(context_));
}

bool SvgRenderer::is_finished() const {
    return impl->finished;
}

void SvgRenderer::stop() {
    impl->requested_to_stop = true;
}

void SvgRenderer::Impl::render(Context&& context_) {
    finished = false;
    rendered_frames_count = 0;
    std::cout << "SvgRenderer start" << std::endl;
    context = std::move(context_);
    document = context.get_document();
    if (!document) {
        throw RenderFailure("No document present");
    }
    boost::any maybe_settings = context.get_render_settings();
    if (!maybe_settings.empty()) {
        settings = boost::any_cast<SvgRendererSettings>(maybe_settings);
    }
    prepare_render();
    for (auto time : context.get_period()) {
        if (requested_to_stop)
            break;
        auto ctx = std::make_shared<Context>(context);
        ctx->set_time(time);
        render_frame(ctx);
        ++rendered_frames_count;
    }
    finish_render();
    std::cout << "SvgRenderer done" << std::endl;
    finished = true;
}

void SvgRenderer::Impl::prepare_render() {
    if (!boost::filesystem::exists(render_path)) {
        if (!boost::filesystem::create_directory(render_path))
            throw RenderFailure("Cannot create render directory");
    }
    if (!settings.path.empty()) {
        base_path = settings.path;
        base_path.remove_filename();
        boost::filesystem::current_path(base_path);
        if (settings.render_pngs)
            restart_png();
    }
    svgs_finished = false;
    if (settings.render_pngs)
        start_png();
}

void SvgRenderer::Impl::render_frame(std::shared_ptr<Context> context) {
    auto time = context->get_time();
    auto base_name = "renders/{:.3f}"_format(time.get_seconds());
    auto svg_name = base_name+".svg";
    std::cout << svg_name << std::endl;
    std::ofstream f(svg_name);
    auto size = document->get_size()->get(context);
    auto viewport_size = document->get_property_value<Geom::Point>("_svg_viewport_size", context).value_or(size);
    auto definitions = document->get_property_value<std::string>("_svg_definitions", context).value_or("");
    fmt::print(f, svg_template, size.x(), size.y(), viewport_size.x(), viewport_size.y(), definitions, frame_to_svg(context));
    f.close();
    if (settings.render_pngs)
        render_png(svg_name, (base_path / (base_name+".png")).string());
    parent->finished_frame()(time);
}

std::string SvgRenderer::Impl::frame_to_svg(std::shared_ptr<Context> context) const {
    return node_to_svg({document->get_root(), context});
}

std::string get_extra_style(AbstractNode const& node, std::shared_ptr<Context> ctx, SvgRendererSettings const& settings) {
    if (settings.extra_style)
        return node.get_property_value<std::string>("_svg_style", ctx).value_or("");
    return "";
}

std::string node_to_svg(NodeInContext nic, SvgRendererSettings const& settings) {
    auto node_ptr = nic.node;
    auto context = nic.context;
    if (!node_ptr)
        return "";
    if (node_ptr->get_type() != typeid(Renderable)) {
        std::cerr << "ERROR: node isn't renderable" << std::endl;
        // throw
        return "";
    }
    auto node = dynamic_cast<AbstractNode*>(node_ptr.get());
    auto name = node_name(*node_ptr);
    try {
        return class_init::name_info<SvgRendererModule>(name)(*node, context, settings);
    } catch (class_init::TypeLookupError const&) {
        if (auto proxy = dynamic_cast<ProxyNode<Renderable>*>(node)) {
            std::string result;
            result = node_to_svg(proxy->get_proxy(context), settings);
            return result;
        }
        std::cerr << "ERROR: node type isn't supported" << std::endl;
        // throw
        return "";
    }
}

std::string SvgRenderer::Impl::node_to_svg(NodeInContext nic) const {
    return renderers::node_to_svg(nic, settings);
}

void SvgRenderer::Impl::finish_render() {
    svgs_finished = true;
    document.reset();
    if (settings.render_pngs)
        quit_png();
    requested_to_stop = false;
}

/**
 * Pipe, fork & exec
 *
 * @return pid of new process
 */
pid_t fork_pipe(FILE*& write_pipe, FILE*& read_pipe, std::vector<std::string> args) {
    if (args.size() < 1)
        throw std::invalid_argument("Empty argument list");

    int write_pipe_ds[2];
    pipe(write_pipe_ds);
    int read_pipe_ds[2];
    pipe(read_pipe_ds);

    auto pid = fork();
    if (pid == 0) {
        dup2(write_pipe_ds[0], STDIN_FILENO);
        close(write_pipe_ds[0]);
        close(write_pipe_ds[1]);

        dup2(read_pipe_ds[1], STDOUT_FILENO);
        dup2(read_pipe_ds[1], STDERR_FILENO);
        close(read_pipe_ds[0]);
        close(read_pipe_ds[1]);

        char const** c_args = new char const*[args.size()+1];
        std::transform(
            std::begin(args),
            std::end(args),
            c_args,
            [](auto const& s) {
                return s.c_str();
            }
        );
        c_args[args.size()] = nullptr;
        execv(args[0].c_str(), const_cast<char* const*>(c_args));
        throw std::runtime_error("execl returned");
    }

    write_pipe = fdopen(write_pipe_ds[1], "w");
    close(write_pipe_ds[0]);

    fcntl(read_pipe_ds[0], F_SETFL, O_NONBLOCK);
    read_pipe = fdopen(read_pipe_ds[0], "r");
    close(read_pipe_ds[1]);

    return pid;
}

void SvgRenderer::Impl::start_png(bool force) {
    if (subprocess_initialized) {
        if (!force)
            return;
        quit_png(true);
    }

    png_renderer_pid = fork_pipe(png_renderer_pipe, png_renderer_pipe_output, {"/usr/bin/env", "inkscape", "--shell", "-z"});

    read_thread = std::thread([this]() {
        auto buff = std::make_unique<char[]>(256);
        std::string sbuff;
        size_t frames_count = 0;
        while (!svgs_finished || frames_count < rendered_frames_count) {
            // TODO: replace with blocking read?
            std::this_thread::sleep_for(std::chrono::milliseconds(64));

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
        }
    });

    subprocess_initialized = true;
}

void SvgRenderer::Impl::render_png(std::string const& svg, std::string const& png) {
    fputs("{} {} {}\n"_format(svg, "-e", png).c_str(), png_renderer_pipe);
    fflush(png_renderer_pipe);
}

void SvgRenderer::Impl::quit_png(bool force) {
    bool quit_inkscape = force || !settings.keep_alive || requested_to_stop;
    if (quit_inkscape) {
        subprocess_initialized = false;

        fputs("quit\n", png_renderer_pipe);
        fflush(png_renderer_pipe);
    }

    // now wait until inkscape renders all the frames..
    if (read_thread.joinable())
        read_thread.join();

    if (quit_inkscape) {
        int status;
        waitpid(png_renderer_pid, &status, 0);
    }
}

} // namespace filters

} // namespace rainynite::core
