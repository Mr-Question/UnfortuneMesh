cmake_minimum_required(VERSION 3.0.0)
project(UnfortuneMesh VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

set (PROJECT_ROOT_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE_FILES
  "${PROJECT_ROOT_DIR}/*.cpp"
  "${PROJECT_ROOT_DIR}/*.h")

include_directories(${PROJECT_ROOT_DIR})

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
