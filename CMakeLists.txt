cmake_minimum_required(VERSION 3.15)
project(SolverCO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(BUILD_SOLVERCO_ALL "Build all library by default" ON)

macro(solverco_module_setup _var_name _description)
    if(BUILD_SOLVERCO_ALL)
        set(${_var_name} ON CACHE BOOL "${_description}" FORCE)
    else()
        if(NOT DEFINED ${_var_name})
            set(${_var_name} OFF CACHE BOOL "${_description}")
        endif()
    endif()
    if(NOT ${_var_name})
        message(STATUS "Module ${_var_name}: DISABLED")
        return()
    endif()
    message(STATUS "Module ${_var_name}: ENABLED")
endmacro()

set(SOLVERCO_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/src_build")

find_package(Doxygen)

if (DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()
