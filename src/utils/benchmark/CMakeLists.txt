solverco_module_setup(BUILD_UTILS_BENCHMARK "Build benchmark module")

add_library(benchmark_lib STATIC
    benchmark_result.cpp
    benchmark_visualizer.cpp
    benchmark_manager.cpp
)

target_include_directories(benchmark_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
)

target_compile_options(benchmark_lib PRIVATE -Wall -Wextra -Wpedantic)
