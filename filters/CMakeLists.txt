add_library(filters SHARED)

target_sources(filters
    PRIVATE
        $<TARGET_OBJECTS:yaml-cpp>
        json_reader.cpp
        json_writer.cpp
)

target_link_libraries(filters PUBLIC document geom_helpers)

install(
    TARGETS filters
    EXPORT rainynite_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
