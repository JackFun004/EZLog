cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_STANDARD 17)
project(ezlog VERSION 0.1.0 LANGUAGES C CXX)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_library(ezlog ${SRC})

add_executable(main ${SRC} main.cpp)
target_link_libraries(main pthread)

include(CTest)
enable_testing()

add_subdirectory(${CMAKE_SOURCE_DIR}/tests)




