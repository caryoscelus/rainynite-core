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
#include "../renderer.h"

namespace rainynite::core {

namespace renderers {

struct SvgRendererSettings {
    bool render_pngs = false;
    bool keep_alive = false;
    bool extra_style = true;
    std::string path;
};

class SvgRenderer : public Renderer {
public:
    SvgRenderer();
    virtual ~SvgRenderer();
public:
    void render(Context&& context) override;
    bool is_finished() const override;
    void stop() override;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace renderers

} // namespace rainynite::core

#endif
