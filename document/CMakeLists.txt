add_library(document SHARED)

target_sources(document
    PRIVATE
        context.cpp
        document.cpp
        document_loader.cpp
        instance.cpp
        default_nodes.cpp
        empty.cpp
        color.cpp
        shading.cpp
        action.cpp
        action_stack.cpp
        node_info.cpp
        node_tree.cpp
        node_tree_transform.cpp
        node/nic.cpp
        io.cpp
)

target_link_libraries(document log morphing fmt time stdc++fs)

add_subdirectory(nodes)

install(
    TARGETS document
    EXPORT rainynite_core
    LIBRARY DESTINATION lib/rainynite/
    ARCHIVE DESTINATION lib/rainynite/
)
