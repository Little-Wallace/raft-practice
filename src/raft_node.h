//
// Created by liuwei on 4/29/18.
//

#ifndef RAFT_RAFT_NODE_H
#define RAFT_RAFT_NODE_H
#include <string>
#include <memory>
#include "node_client.h"

class RaftNode {
public:
    RaftNode(size_t id_, const std::string& address);
    ~RaftNode() {}
    bool Init();
    uint64_t GetId() {
        return (uint64_t)_id;
    }
private:
    size_t _id;
    std::string _address;
    NodeClient _client;
};


#endif //RAFT_RAFT_NODE_H
