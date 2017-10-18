add_library(time SHARED)

target_sources(time
    PRIVATE
        time.cpp
        parse.cpp
        format.cpp
)

target_link_libraries(time morphing)

install(
    TARGETS time
    EXPORT rainynite_core
    LIBRARY DESTINATION lib/rainynite/
    ARCHIVE DESTINATION lib/rainynite/
)
