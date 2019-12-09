cmake_minimum_required(VERSION 3.15)
project(GProject)

find_package(Boost COMPONENTS thread)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

find_package(Protobuf)
IF (Protobuf_FOUND)
    include_directories(${PROTOBUF_INCLUDE_DIR})
ENDIF()

include_directories(${CMAKE_CURRENT_BINARY_DIR})


configure_file(CMakeLists.txt.in
googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
${CMAKE_BINARY_DIR}/googletest-build)


set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

include_directories(include)
IF(Protobuf_FOUND)
    protobuf_generate_cpp(PROTO_SRC PROTO_HEADER protobuf/structures.proto)
ELSE()
    set(PROTO_HEADER include/structures.pb.h)
    set(PROTO_SRC src/structures.pb.cc)
endif()

add_library(proto ${PROTO_HEADER} ${PROTO_SRC})

add_library(algorithm STATIC src/algorithm.cpp)

add_library(queue STATIC src/queue.cpp)

add_library(server STATIC src/server.cpp)
target_link_libraries(server proto ${Boost_LIBRARIES})

add_library(worker STATIC src/worker.cpp)

add_executable(test test/test.cpp)
IF(Protobuf_FOUND)
    target_link_libraries(test gtest gmock server proto ${PROTOBUF_LIBRARY})
ELSE()
    target_link_libraries(test gtest gmock server proto)
ENDIF()