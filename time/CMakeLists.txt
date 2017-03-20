add_library(time SHARED)

target_sources(time
    PRIVATE
        time.cpp
)

target_link_libraries(time morphing)

install(
    TARGETS time
    EXPORT time
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

export(
    EXPORT time
    FILE "${CMAKE_CURRENT_BINARY_DIR}/time-config.cmake"
)

install(
    EXPORT time
    FILE "time-config.cmake"
    DESTINATION "lib/cmake/time"
)
