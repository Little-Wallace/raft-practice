//
// Created by liuwei on 5/1/18.
//

#ifndef RAFT_STATE_MACHINE_H
#define RAFT_STATE_MACHINE_H
#include "common.h"
#include "raft_node.h"

#include "util/array_lock_free_queue.h"


template<typename T>
class RaftStateMachine
{
public:
    typedef T RaftLog;
    RaftStateMachine(RaftLog* log,
                     const std::vector<RaftNode>& nodes)
        : _log(log)
        , _nodes(nodes)
    {}
    ~RaftStateMachine() {}
    bool Step(const ::Node::RaftMessage& response) {
        assert(false);
        return false;
    }

private:
    RaftLog* _log;
    std::vector<RaftNode> _nodes;
    
};

#endif

