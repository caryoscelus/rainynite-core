add_library(log SHARED)

target_sources(log
    PRIVATE
        simple_exception_log.cpp
)

target_link_libraries(log fmt)

install(
    TARGETS log
    EXPORT rainynite_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
