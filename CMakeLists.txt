cmake_minimum_required(VERSION 3.0.0)
project(UnfortuneMesh VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

set (PROJECT_ROOT_DIR            "${CMAKE_CURRENT_SOURCE_DIR}/src")
set (PROJECT_SUBMODULES_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules")

add_subdirectory ("${PROJECT_SUBMODULES_ROOT_DIR}/UnfortuneBase")

file(GLOB_RECURSE SOURCE_FILES
  "${PROJECT_ROOT_DIR}/*.cpp"
  "${PROJECT_ROOT_DIR}/*.h")

include_directories(${PROJECT_ROOT_DIR})
include_directories(${UnfortuneBase_INCLUDE_DIRS})

add_executable  (${PROJECT_NAME} main.cpp ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} UnfortuneBase)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
