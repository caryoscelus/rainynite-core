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

find_package(geom_helpers REQUIRED)
find_package(morphing REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

enable_testing()
add_subdirectory(tests)

add_subdirectory(document)
add_subdirectory(filters)
add_subdirectory(time)
add_subdirectory(renderers)

install(
    DIRECTORY include/
    DESTINATION include
)
