function(add_catch_test name)
    add_executable("test_${name}" main.cpp "${name}.cpp")
    add_test("test_${name}" "test_${name}")
    target_include_directories("test_${name}"
        SYSTEM BEFORE PUBLIC
            ${CATCH_INCLUDE_DIRS}
    )
endfunction()

add_catch_test(time)
target_link_libraries(test_time time)

add_catch_test(node)
add_catch_test(class_init)

add_catch_test(serialize)
target_link_libraries(test_serialize document time)

add_catch_test(rapidjson)
target_link_libraries(test_rapidjson document time)

add_catch_test(parse)
target_link_libraries(test_parse time)
