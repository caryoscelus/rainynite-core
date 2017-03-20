add_library(renderers SHARED)

target_sources(renderers
    PRIVATE
        svg_renderer.cpp
)

find_package(time REQUIRED)

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
