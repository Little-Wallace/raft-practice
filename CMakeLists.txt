cmake_minimum_required(VERSION 3.7)
project(raft)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)
link_directories("/usr/local/lib")
link_libraries(pthread)
link_libraries(grpc)
link_libraries(protobuf)
link_libraries(grpc++)
link_libraries(gflags)
set(SOURCES
        src/raft_node.cpp src/node_server_service_impl.cpp
        src/node_client.cpp src/util/string_util.cpp
        src/proto/message.grpc.pb.cc src/proto/message.pb.cc)
add_library(raftlib STATIC ${SOURCES})
INCLUDE_DIRECTORIES(./src)
link_libraries(raftlib)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)
# target_link_libraries(server raftlib)
# target_link_libraries(client proto)
# target_link_libraries(client raftlib)
