cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test_mis test.cpp)

target_compile_definitions(test_mis PUBLIC
    PATH_GRAPH_INSTANCES="${SOLVERCO_ROOT_PATH}/instances/graph_instances/"
)

if(NOT TARGET mis_lib)
    set(BUILD_SOLVERS_MIS ON CACHE BOOL "Enable MIS module" FORCE)
    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/solvers/graph_solvers/select_vertices/mis ${CMAKE_CURRENT_BINARY_DIR}/mis_build)
endif()

if (NOT TARGET benchmark_lib)
    set(BUILD_UTILS_BENCHMARK ON CACHE BOOL "Enable benchmark module" FORCE)
    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/utils/benchmark ${CMAKE_CURRENT_BINARY_DIR}/benchmark_build)
endif()

target_link_libraries(test_mis PRIVATE mis_lib benchmark_lib)
