add_executable(test_time main.cpp time.cpp)
target_link_libraries(test_time time)
target_include_directories(test_time
    SYSTEM BEFORE PUBLIC
        ${CATCH_INCLUDE_DIRS}
)
add_test(test_time test_time)

add_executable(test_node main.cpp node.cpp)
# target_link_libraries(test_node)
target_include_directories(test_node
    SYSTEM BEFORE PUBLIC
        ${CATCH_INCLUDE_DIRS}
)
add_test(test_node test_node)
