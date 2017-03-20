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
};

class SvgRenderer : public Renderer {
public:
    virtual void render(Context context) override;
    virtual bool is_finished() override;
private:
    void prepare_render();
    void render_frame(Time time);
    Geom::BezierKnots morph_path(Time time);
private:
    bool finished = false;
    Context context;
    std::shared_ptr<Document> document;
private:
    Geom::BezierKnots from;
    Geom::BezierKnots to;
    SvgRendererSettings settings;
};

} // namespace renderers

} // namespace core

#endif
