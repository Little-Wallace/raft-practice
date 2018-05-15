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
    enum RaftNodeStatus {
        Probe,
        Replicate,
        Snapshot
    };
public:
    RaftNode(size_t id_, const std::string& address);
    ~RaftNode() {}
    bool Init();
    bool IsFull() const {
        assert(false);
        return false;
    };
    uint64_t GetId() const {
        return (uint64_t)_id;
    }
    RaftNodeStatus GetState() const {
        return _state;
    }
    int64_t GetMatched() const {
        return _matched;
    }
    bool GetRecentActive() {
        return _recent_active;
    }

    void Resume() {
        assert(false);
    }
    void SetRecentActive(bool active) {
        _recent_active = active;
    }
    void MaybeUpdate(int64_t idx) {
        assert(false);
    }
private:
    size_t _id;
    int64_t _matched;
    RaftNodeStatus _state;
    bool _recent_active;
    std::string _address;

};


#endif //RAFT_RAFT_NODE_H
