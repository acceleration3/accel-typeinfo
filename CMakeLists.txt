cmake_minimum_required(VERSION 3.10)

project(accel-typeinfo CXX)

set(ACCEL_BUILD_TESTS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

if(NOT DEFINED ACCEL_MODULES_FOLDER)
    set(ACCEL_MODULES_FOLDER "${CMAKE_SOURCE_DIR}/external")
endif()

set(ADDITIONAL_LIBRARIES accel-macros accel-compat)

include(cmake/FindModule.cmake)
add_accel_module(accel-macros)
add_accel_module(accel-compat)

add_library(accel-typeinfo INTERFACE)
target_include_directories(accel-typeinfo INTERFACE "include/")
target_link_libraries(accel-typeinfo INTERFACE ${ADDITIONAL_LIBRARIES})

if(ACCEL_BUILD_TESTS)
    add_subdirectory(tests)
endif()