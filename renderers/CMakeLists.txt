add_library(renderers SHARED)

## TODO: move svg_renderer to proper subdir
## for some reason it doesn't work to simply link renderers with svg_renderer:
## when installed, librenderers.so cannot find libsvg_renderer.so...

set(BUILD_SVG_RENDERER_MODULES ON CACHE BOOL "Whether to build actual SVG renderer modules (useful to turn off to just run basic tests)")

target_sources(renderers
    PRIVATE
        svg_renderer/svg_renderer.cpp
        svg_renderer/svg_module.h
        svg_renderer/shape.h
)

if (BUILD_SVG_RENDERER_MODULES)
    target_sources(renderers
        PRIVATE
            svg_renderer/empty.cpp
            svg_renderer/image.cpp
            svg_renderer/text.cpp
            svg_renderer/transform.cpp
            svg_renderer/composite.cpp
            svg_renderer/render_shape.cpp
            svg_renderer/shapes.cpp
    )
endif()

if (NOT TARGET time)
    find_package(time REQUIRED)
endif()

target_link_libraries(renderers
    document
    time
    geom_helpers
    boost_system
    boost_filesystem
)

install(
    TARGETS renderers
    EXPORT rainynite_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

