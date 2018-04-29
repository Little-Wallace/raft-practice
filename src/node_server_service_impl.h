//
// Created by liuwei on 4/23/18.
//

#ifndef RAFT_RAWNODE_H
#define RAFT_RAWNODE_H

#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include "proto/message.grpc.pb.h"
#include "raft_node.h"

class NodeServerServiceImpl final : public Node::NodeServer::Service {
public:
    NodeServerServiceImpl(
            const std::string& address,
            const std::vector<std::string>& clusters);
    ~NodeServerServiceImpl();
    grpc::Status Raft(grpc::ServerContext* context,
                        const Node::HardState* request,
                        Node::Done* response) override
    {
        assert(false);
    }
    grpc::Status AddLog(grpc::ServerContext* context,
                        const Node::PutRequest* request,
                        Node::Done* response) override
    {
        const std::string& buffer = request->content();
        std::cout << "node " << _id << "receive " << buffer << std::endl;
        return grpc::Status::OK;
    }

private:
    bool _run;
    size_t _id;
    std::thread* _workLoop;
};

#endif //RAFT_RAWNODE_H
