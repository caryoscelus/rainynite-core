add_library(filters SHARED)

target_sources(filters
    PRIVATE
        json_reader.cpp
        json_writer.cpp
        yaml_reader.cpp
        yaml_writer.cpp
)

target_link_libraries(filters PUBLIC document geom_helpers)
target_link_libraries(filters PRIVATE yaml-cpp)

install(
    TARGETS filters
    EXPORT rainynite_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
