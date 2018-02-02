function(add_catch_test name)
    add_executable("test_${name}" main.cpp "${name}.cpp")
    add_test("test_${name}" "test_${name}")
    target_include_directories("test_${name}"
        SYSTEM BEFORE PUBLIC
            ${CATCH_INCLUDE_DIRS}
    )
endfunction()

add_catch_test(nullptr)

add_catch_test(crtp)

add_catch_test(class_init)

add_catch_test(time)
target_link_libraries(test_time time)

add_catch_test(node)
target_link_libraries(test_node nodes)

add_catch_test(node_tree)
target_link_libraries(test_node_tree document)

add_catch_test(interpolate_node)
target_link_libraries(test_interpolate_node nodes)

add_catch_test(node_tree_modules)
target_link_libraries(test_node_tree_modules document)

add_catch_test(serialize)
target_link_libraries(test_serialize nodes)

add_catch_test(parse)
target_link_libraries(test_parse time)

add_catch_test(actions)
target_link_libraries(test_actions nodes)

add_catch_test(notify)
target_link_libraries(test_notify document)

add_catch_test(list_nodes)
target_link_libraries(test_list_nodes nodes)

add_catch_test(time_nodes)
target_link_libraries(test_time_nodes nodes)

add_catch_test(dynamic_node)
target_link_libraries(test_dynamic_node nodes)

add_catch_test(type_constraint)

add_catch_test(link_storage)
target_link_libraries(test_link_storage document)

add_catch_test(new_node)
target_link_libraries(test_new_node nodes)

option(ENABLE_RNITE_DOCUMENT_TESTER "Enable .rnite file testing" ON)

if (ENABLE_RNITE_DOCUMENT_TESTER)
    ## New node tests
    add_executable(node_tester node_tester.cpp)
    target_link_libraries(node_tester nodes filters)
    function(add_node_test name)
        add_test(node_tester_${name} node_tester "${CMAKE_CURRENT_SOURCE_DIR}/data/${name}.rnite")
    endfunction()

    add_node_test(sanity)
    add_node_test(serialize)
    add_node_test(boolean)
    add_node_test(dynamic)
    add_node_test(numeric)
    add_node_test(frames)
    add_node_test(interpolate)
    add_node_test(notags)
    add_node_test(to_string)
    add_node_test(string)
    add_node_test(transform_tree)
    add_node_test(point)
    add_node_test(list)
    add_node_test(sorting)
    add_node_test(custom_property)
    add_node_test(zdepth)
endif()
