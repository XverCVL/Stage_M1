solverco_module_setup(BUILD_SOLVERS_VCP "Build VCP module")

add_library(vcp_lib STATIC
    solution_vcp.cpp
    vcp_solvers/vcp_colpack/vcp_colpack.cpp
    vcp_solvers/vcp_tabu/tabucol.cpp
)

if(NOT TARGET graph_lib)
    set(BUILD_UTILS_GRAPH ON CACHE BOOL "Enable graph module" FORCE)
    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/utils/graph ${CMAKE_CURRENT_BINARY_DIR}/graph_build)
endif()

#if(NOT TARGET branch_and_bound_lib)
#    set(BUILD_ALGORITHMS_BRANCH_AND_BOUND ON CACHE BOOL "Enable branch&bound module" FORCE)
#    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/algorithms/branch&bound ${CMAKE_CURRENT_BINARY_DIR}/branch&bound_build)
#endif()

target_link_libraries(vcp_lib PUBLIC graph_lib)
#target_link_libraries(vcp_lib PUBLIC branch_and_bound_lib)

target_include_directories(vcp_lib SYSTEM PUBLIC ${SOLVERCO_ROOT_PATH}/external_libraries)

target_include_directories(vcp_lib SYSTEM PUBLIC
    ${SOLVERCO_ROOT_PATH}/external_libraries/ColPack/include
)

target_include_directories(vcp_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vcp_solvers/vcp_tabu
    ${CMAKE_CURRENT_SOURCE_DIR}/vcp_solvers/vcp_colpack
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../utils/containers
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../components
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_options(vcp_lib PRIVATE -Wall -Wextra -Wpedantic)
