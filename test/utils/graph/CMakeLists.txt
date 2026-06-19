cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test_graph test.cpp)

target_compile_definitions(test_graph PUBLIC
    PATH_GRAPH_INSTANCES="${SOLVERCO_ROOT_PATH}/instances/graph_instances/"
)

if(NOT TARGET graph_lib)
    set(BUILD_UTILS_GRAPH ON CACHE BOOL "Enable graph module" FORCE)
    add_subdirectory(${SOLVERCO_ROOT_PATH}/utils/graph ${CMAKE_CURRENT_BINARY_DIR}/graph_build)
endif()

target_include_directories(test_graph PRIVATE ${SOLVERCO_ROOT_PATH}/src
    ${SOLVERCO_ROOT_PATH}/src/solvers/graph_solvers/components
)

target_link_libraries(test_graph PRIVATE graph_lib)
