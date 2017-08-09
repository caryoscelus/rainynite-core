add_library(document SHARED)

target_sources(document
    PRIVATE
        context.cpp
        document.cpp
        instance.cpp
        color.cpp
        node/abstract_node.cpp
        nodes/animated.cpp
        nodes/frame_list.cpp
        nodes/time.cpp
        nodes/time_list.cpp
        nodes/time_period.cpp
        nodes/dynamic_node.cpp
        nodes/morph.cpp
        nodes/format_string.cpp
        nodes/color_mix.cpp
        nodes/follow_path.cpp
        nodes/path_xy.cpp
        nodes/extract_coord.cpp
        nodes/point_xy.cpp
        nodes/knot.cpp
        nodes/knot_list.cpp
        nodes/linear.cpp
        nodes/truncate.cpp
        nodes/to_string.cpp
        nodes/color_string.cpp
        nodes/file_string.cpp
        nodes/math.cpp
        nodes/rectangle.cpp
        nodes/list.cpp
        nodes/random.cpp
        nodes/renderable/empty.cpp
        nodes/renderable/composite.cpp
        nodes/renderable/image.cpp
        nodes/renderable/translate.cpp
        nodes/renderable/scale.cpp
        nodes/renderable/rotate.cpp
        nodes/renderable/text.cpp
        nodes/renderable/render_shape.cpp
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
