add_library(renderers SHARED)

## TODO: move svg_renderer to proper subdir
## for some reason it doesn't work to simply link renderers with svg_renderer:
## when installed, librenderers.so cannot find libsvg_renderer.so...

target_sources(renderers
    PRIVATE
        svg_renderer/svg_renderer.cpp
        svg_renderer/svg_module.h
        svg_renderer/shape.h

        svg_renderer/image.cpp
        svg_renderer/text.cpp
        svg_renderer/transform.cpp
        svg_renderer/composite.cpp
        svg_renderer/render_shape.cpp
        svg_renderer/shapes.cpp
)

if (NOT TARGET time)
    find_package(time REQUIRED)
endif()

target_link_libraries(renderers
    document
    time
    geom_helpers
)

install(
    TARGETS renderers
    EXPORT renderers
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

export(
    EXPORT renderers
    FILE "${CMAKE_CURRENT_BINARY_DIR}/renderers-config.cmake"
)

install(
    EXPORT renderers
    FILE "renderers-config.cmake"
    DESTINATION "lib/cmake/renderers"
)
