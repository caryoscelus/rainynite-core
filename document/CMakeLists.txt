add_library(document SHARED)

target_sources(document
    PRIVATE
        context.cpp
        document.cpp
        instance.cpp
        color.cpp
        nodes/animated.cpp
        nodes/morph.cpp
        nodes/path_shape.cpp
        nodes/rectangle_shape.cpp
        nodes/image.cpp
        nodes/translate.cpp
        nodes/text.cpp
        nodes/format_string.cpp
        nodes/color_mix.cpp
        nodes/follow_path.cpp
        nodes/extract_coord.cpp
        nodes/point_xy.cpp
        nodes/linear.cpp
        nodes/to_string.cpp
        nodes/color_string.cpp
        nodes/file_string.cpp
)

target_link_libraries(document morphing fmt)

install(
    TARGETS document
    EXPORT document
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

export(
    EXPORT document
    FILE "${CMAKE_CURRENT_BINARY_DIR}/document-config.cmake"
)

install(
    EXPORT document
    FILE "document-config.cmake"
    DESTINATION "lib/cmake/document"
)
