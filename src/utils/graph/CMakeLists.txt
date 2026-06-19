solverco_module_setup(BUILD_UTILS_GRAPH "Build graph module")

add_library(graph_lib STATIC
    graph.cpp
    graph_list_adj_no.cpp
    graph_heavy_no.cpp
    graph_utils.cpp
)

target_include_directories(graph_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(graph_lib PRIVATE -Wall -Wextra -Wpedantic)
