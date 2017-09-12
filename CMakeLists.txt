cmake_minimum_required(VERSION 3.8)

project(rainynite-core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)
pkg_check_modules(LIB2GEOM REQUIRED 2geom)

find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(FMT REQUIRED)

set(CATCH_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/catch/" CACHE STRING "Path to Catch header library")
set(RAPIDJSON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/rapidjson/include/" CACHE STRING "Path to RapidJSON header library")

include_directories(SYSTEM "${RAPIDJSON_INCLUDE_DIRS}")

set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "YAML: Enable testing and parse tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "YAML: Enable contrib stuff in library")
add_subdirectory(thirdparty/yaml-cpp)

find_package(geom_helpers REQUIRED)
find_package(morphing REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

enable_testing()
add_subdirectory(tests)

add_subdirectory(log)
add_subdirectory(document)
add_subdirectory(filters)
add_subdirectory(time)
add_subdirectory(renderers)

export(
    EXPORT rainynite_core
    FILE "${CMAKE_CURRENT_BINARY_DIR}/rainynite_core-config.cmake"
)

install(
    EXPORT rainynite_core
    FILE "rainynite_core-config.cmake"
    DESTINATION "lib/cmake/rainynite_core"
)

install(
    DIRECTORY include/
    DESTINATION include
)
