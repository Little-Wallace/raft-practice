//
// Created by liuwei on 4/23/18.
//

#ifndef RAFT_NodeClient_H
#define RAFT_NodeClient_H

#include <string>
#include <memory>

#include "message.grpc.pb.h"


class NodeClient {
public:
    enum ResStatus{
        OK,
        TIMEOUT,
        ERROR
    };
    NodeClient() {}
    ~NodeClient() {}
    bool Init(const std::string& address);
    bool Add(const std::string& content);
    ResStatus Send(const Node::RaftMessage& message);
private:
    std::unique_ptr<Node::NodeServer::Stub> _stub;
    gpr_timespec _timespec;

};

#endif //RAFT_NodeClient_H
