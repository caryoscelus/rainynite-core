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
        nodes/image.cpp
        nodes/translate.cpp
        nodes/text.cpp
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
