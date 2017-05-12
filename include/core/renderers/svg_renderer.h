/*
 *  svg_renderer.h - primitive .svg renderer
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

#ifndef __CORE__RENDERERS__SVG_RENDERER_H__F493F66A
#define __CORE__RENDERERS__SVG_RENDERER_H__F493F66A

#include <memory>

#include "../context.h"
#include "../document.h"
#include "../renderer.h"

namespace core {

namespace renderers {

struct SvgRendererSettings {
    bool render_pngs = false;
    bool extra_style = true;
};

class SvgRenderer : public Renderer {
public:
    virtual void render(Context context) override;
    virtual bool is_finished() override;
private:
    void prepare_render();
    void render_frame(Time time);
    void finish_render();
    std::string definitions(Time time) const;
    std::string frame_to_svg(Time time) const;
    std::string node_to_svg(core::AbstractReference root_ptr, Time time) const;
    void start_png();
    void render_png(std::string const& svg, std::string const& png);
    void quit_png();
private:
    bool finished = false;
    Context context;
    std::shared_ptr<Document> document;
private:
    SvgRendererSettings settings;
    FILE* png_renderer_pipe;
    pid_t png_renderer_pid;
};

} // namespace renderers

} // namespace core

#endif
