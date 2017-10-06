add_library(os SHARED)

target_sources(os
    PRIVATE
        fork_pipe.cpp
)

install(
    TARGETS os
    EXPORT rainynite_core
    LIBRARY DESTINATION lib/rainynite/
    ARCHIVE DESTINATION lib/rainynite/
)
