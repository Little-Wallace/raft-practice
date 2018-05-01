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
    typedef T RaftLog;
    typedef ArrayLockFreeQueue<Node::RaftMessage, 100> RaftMessageQue;
public:
    RaftStateMachine(RaftMessageQue* receiveQueue, RaftLog* log,
                     const std::vector<RaftNode>& nodes)
        : _receiveQueue(receiveQueue)
        , _log(log)
        , _nodes(nodes)
    {}
    ~RaftStateMachine() {}
    bool Start() {
        assert(false);
    }
    bool Step() {
        assert(false);
    }
private:
    RaftMessageQue* _receiveQueue;
    RaftLog* _log;
    std::vector<RaftNode> _nodes;
    
};

#endif

