/*
 *  document.h - document
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

#ifndef __CORE__DOCUMENT_H__62AC426A
#define __CORE__DOCUMENT_H__62AC426A

#include <memory>

#include <geom_helpers/knots.h>

#include <core/time.h>

namespace core {

class Context;

class Document : public std::enable_shared_from_this<Document> {
public:
    explicit Document(std::vector<Geom::BezierKnots> keyframes_=std::vector<Geom::BezierKnots>());
    virtual ~Document();
    std::shared_ptr<Context> get_default_context();

public:
    inline std::vector<Geom::BezierKnots> const& get_keyframes() const {
        return keyframes;
    }
    inline unsigned keyframe_amount() const {
        return keyframes.size();
    }
    inline TimePeriod get_main_time_period() const {
        return main_time_period;
    }

private:
    std::vector<Geom::BezierKnots> keyframes;
    std::shared_ptr<Context> default_context;
    TimePeriod main_time_period;
};

}

#endif
