cmake_minimum_required(VERSION 3.15)
project(GProject)

#find_package(Boost COMPONENTS thread)
#IF (Boost_FOUND)
#    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
#    ADD_DEFINITIONS( "-DHAS_BOOST" )
#ENDIF()


configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

include_directories(include)

add_library(algorithm STATIC src/algorithm.cpp)

add_library(queu STATIC src/queu.cpp)

add_library(server STATIC src/server.cpp)

#find_package (sqlite3.h)

#if (SQLITE3_FOUND)
#    include_directories(${SQLITE3_INCLUDE_DIRS})
#    target_link_libraries (new ${SQLITE3_LIBRARIES})
#endif (SQLITE3_FOUND)
#add_library(sqlite3 STATIC include/sqlite-autoconf-3300100/sqlite3.c)

add_library(worker STATIC src/worker.cpp)

include_directories(/usr/include)
link_directories(/usr/lib)
target_link_libraries(worker gtest gmock sqlite3)

add_executable(test test/test.cpp src/worker.cpp)
target_link_libraries(test gtest gmock)