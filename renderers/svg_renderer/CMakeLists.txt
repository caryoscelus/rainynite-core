add_library(svg_renderer SHARED)

set(BUILD_SVG_RENDERER_MODULES ON CACHE BOOL "Whether to build actual SVG renderer modules (useful to turn off to just run basic tests)")

target_sources(svg_renderer
    PRIVATE
        svg_renderer.cpp
        svg_module.h
        shape.h
)

if (BUILD_SVG_RENDERER_MODULES)
    target_sources(svg_renderer
        PRIVATE
            empty.cpp
            image.cpp
            text.cpp
            transform.cpp
            composite.cpp
            render_shape.cpp
            shapes.cpp
            svg.cpp
    )
endif()

target_link_libraries(svg_renderer
    os
    document
    time
    geom_helpers
    boost_system
    boost_filesystem
)

install(
    TARGETS svg_renderer
    EXPORT rainynite_core
    LIBRARY DESTINATION lib/rainynite/
    ARCHIVE DESTINATION lib/rainynite/
)
