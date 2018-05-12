cmake_minimum_required(VERSION 3.7)
project(raft)

set(CMAKE_CXX_STANDARD 11)
find_package(Threads)

add_definitions(-D__KERNEL__=1)
add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)

link_directories("/usr/local/lib")
INCLUDE_DIRECTORIES("/usr/local/include")
link_libraries(pthread)
link_libraries(grpc)
link_libraries(protobuf)
link_libraries(grpc++)
link_libraries(gflags)
link_libraries(gtest)

set(SOURCES
        src/raft_node.cpp
        src/node_client.cpp src/util/string_util.cpp
        src/service_keeper.cpp src/raft_state_machine.cpp
        src/raft_state_machine_candidate.cpp
        src/raft_state_machine_leader.cpp
        src/node_server_service_impl.cpp
        proto/message.grpc.pb.cc proto/message.pb.cc)
add_library(raft STATIC ${SOURCES})
# add_library(raft SHARED ${SOURCES})
INCLUDE_DIRECTORIES(./src)
INCLUDE_DIRECTORIES(./proto)
link_libraries(raft)

# target_link_libraries(raft)
add_executable(raft_test src/raft_test.cpp)
add_executable(server src/server.cpp)
# add_executable(client src/client.cpp)

