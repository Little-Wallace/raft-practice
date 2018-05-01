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
#include "raft_state_machine.h"
#include "util/array_lock_free_queue.h"

template<typename T>
class NodeServerServiceImpl final : public Node::NodeServer::Service {
public:
    typedef RaftStateMachine<T>::RaftMessageQue RaftMessageQue; 
    NodeServerServiceImpl(
            RaftStateMachine<T>* raft,
            RaftMessageQue* receiveQueue)
        : _raft(raft)
        , _receiveQueue(receiveQueue)
    {}
    ~NodeServerServiceImpl() {}


private:
    RaftStateMachine<T>* _raft;
    RaftMessageQue* _receiveQueue;
};

#endif //RAFT_RAWNODE_H
