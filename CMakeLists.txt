cmake_minimum_required(VERSION 3.1)

project(postsynfig-core)

set(CMAKE_CXX_STANDARD 14) # 17 is not supported by cmake 3.7

include(FindPkgConfig)
pkg_check_modules(LIB2GEOM REQUIRED 2geom)

find_package(FMT REQUIRED)

set(CATCH_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/catch/" CACHE STRING "Path to Catch header library")

enable_testing()

find_package(geom_helpers REQUIRED)
find_package(morphing REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(tests)

add_subdirectory(document)
add_subdirectory(filters)
add_subdirectory(renderers)
add_subdirectory(time)

install(
    DIRECTORY include/
    DESTINATION include
)
