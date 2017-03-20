add_executable(test_time main.cpp time.cpp)
target_link_libraries(test_time time)
target_include_directories(test_time
    SYSTEM BEFORE PUBLIC
        ${CATCH_INCLUDE_DIRS}
)
add_test(test_time test_time)
