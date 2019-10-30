cmake_minimum_required(VERSION 3.15)
project(GProject)

configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)


set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(algorithm STATIC src/algorithm.cpp)

add_library(queu STATIC src/queu.cpp)

add_library(server STATIC src/server.cpp)

add_library(worker STATIC src/worker.cpp)

add_executable(test test/test.cpp)
target_link_libraries(test gtest)