//
// Created by liuwei on 4/23/18.
//

#ifndef RAFT_NodeClient_H
#define RAFT_NodeClient_H

#include <string>
#include <memory>

#include "proto/message.grpc.pb.h"


class NodeClient {
public:
    NodeClient() {}
    ~NodeClient() {}
    bool Init(const std::string& address);
    bool AddLog(const std::string& content);
private:
    std::unique_ptr<Node::NodeServer::Stub> _stub;
};

#endif //RAFT_NodeClient_H
