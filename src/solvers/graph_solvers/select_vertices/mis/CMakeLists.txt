solverco_module_setup(BUILD_SOLVERS_MIS "Build MIS module")

add_library(mis_lib STATIC
    solution_mis.cpp
    mis_solvers/mis_greedy/min_degree_mis_solver.cpp
    mis_solvers/mis_kamis/mis_kamis.cpp
    mis_solvers/mis_stablesolver/mis_stablesolver.cpp
)

if(NOT TARGET graph_lib)
    set(BUILD_UTILS_GRAPH ON CACHE BOOL "Enable graph module" FORCE)
    add_subdirectory(${SOLVERCO_ROOT_PATH}/src/utils/graph ${CMAKE_CURRENT_BINARY_DIR}/graph_build)
endif()

target_link_libraries(mis_lib PUBLIC graph_lib)

target_include_directories(mis_lib SYSTEM PUBLIC ${SOLVERCO_ROOT_PATH}/external_libraries)

target_include_directories(mis_lib SYSTEM PUBLIC
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/evolutionary
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/ils
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/initial_mis
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/kernel
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/kernel/ParFastKer/fast_reductions/extern/KaHIP/lib/data_structure
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/kernel/ParFastKer/fast_reductions/extern/KaHIP/lib/tools
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis/kernel/ParFastKer/fast_reductions/extern/KaHIP/lib
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/include/mis
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/extern/KaHIP/interface
    ${SOLVERCO_ROOT_PATH}/external_libraries/KaMIS/extern/KaHIP/lib/io
)

target_include_directories(mis_lib SYSTEM PUBLIC
    ${SOLVERCO_ROOT_PATH}/external_libraries/StableSolver/extern
    ${SOLVERCO_ROOT_PATH}/external_libraries/StableSolver/include
)

target_include_directories(mis_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mis_solvers/mis_stablesolver
    ${CMAKE_CURRENT_SOURCE_DIR}/mis_solvers/mis_kamis
    ${CMAKE_CURRENT_SOURCE_DIR}/mis_solvers/mis_greedy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../components
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_options(mis_lib PRIVATE -Wall -Wextra -Wpedantic)
