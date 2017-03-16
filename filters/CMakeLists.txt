add_library(filters SHARED)

target_sources(filters
    PRIVATE
        svg_path_reader.cpp
        svg_writer.cpp
)

target_link_libraries(filters document geom_helpers)

install(
    TARGETS filters
    EXPORT filters
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

export(
    EXPORT filters
    FILE "${CMAKE_CURRENT_BINARY_DIR}/filters-config.cmake"
)

install(
    EXPORT filters
    FILE "filters-config.cmake"
    DESTINATION "lib/cmake/filters"
)
