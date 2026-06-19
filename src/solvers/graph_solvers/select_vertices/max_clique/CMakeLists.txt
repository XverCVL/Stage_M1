solverco_module_setup(BUILD_SOLVERS_MAX_CLIQUE "Build Max Clique module")

add_library(max_clique_lib STATIC
    solution_max_clique.cpp
    clique_solvers/clique_clisat/clisat_solver.cpp
    clique_solvers/clique_stablesolver/clique_stablesolver.cpp
)

if(NOT TARGET graph_lib)
    set(BUILD_UTILS_GRAPH ON CACHE BOOL "Enable graph module" FORCE)
    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/utils/graph ${CMAKE_CURRENT_BINARY_DIR}/graph_build)
endif()

#if(NOT TARGET branch_and_bound_lib)
#    set(BUILD_ALGORITHMS_BRANCH_AND_BOUND ON CACHE BOOL "Enable branch&bound module" FORCE)
#    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/algorithms/branch&bound ${CMAKE_CURRENT_BINARY_DIR}/branch&bound_build)
#endif()

target_link_libraries(max_clique_lib PUBLIC graph_lib)
#target_link_libraries(max_clique_lib PUBLIC branch_and_bound_lib)

target_include_directories(max_clique_lib SYSTEM PUBLIC ${SOLVERCO_ROOT_PATH}/external_libraries)

target_include_directories(max_clique_lib SYSTEM PUBLIC
    ${SOLVERCO_ROOT_PATH}/external_libraries/StableSolver/extern
    ${SOLVERCO_ROOT_PATH}/external_libraries/StableSolver/include
)

target_include_directories(max_clique_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/clique_solvers/clique_stablesolver
    ${CMAKE_CURRENT_SOURCE_DIR}/clique_solvers/clique_clisat
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../components
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_options(max_clique_lib PRIVATE -Wall -Wextra -Wpedantic)
