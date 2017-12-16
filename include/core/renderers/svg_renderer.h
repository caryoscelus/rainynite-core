/*  svg_renderer.h - SVG "renderer""
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

#ifndef CORE_RENDERERS_SVG_RENDERER_H_F428317B_656B_5ADE_885E_628657C57C69
#define CORE_RENDERERS_SVG_RENDERER_H_F428317B_656B_5ADE_885E_628657C57C69

#include <core/std/memory.h>
#include "../context.h"
#include "../renderer.h"

namespace rainynite::core::renderers {

struct SvgRendererSettings {
    bool render_pngs = false;
    bool keep_alive = false;
    bool extra_style = true;
    string path;
};

/**
 * Svg "renderer" - converts frames to .svg and optionally .png
 */
class SvgRenderer : public Renderer {
public:
    SvgRenderer();
    virtual ~SvgRenderer();

    void render(Context&& context) override;
    bool is_finished() const override;
    void stop() override;

    static string get_rendered_frame_path(Time time, string ext="");

private:
    constexpr static char const* const render_path_base = "renders";

    struct Impl;
    unique_ptr<Impl> impl;
};

} // namespace rainynite::core::renderers

#endif
