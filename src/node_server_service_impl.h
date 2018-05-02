//
// Created by liuwei on 4/23/18.
//

#ifndef RAFT_RAWNODE_H
#define RAFT_RAWNODE_H

#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include "message.grpc.pb.h"
#include "raft_state_machine.h"
#include "util/array_lock_free_queue.h"

typedef ArrayLockFreeQueue<Raft::RaftMessage, 100> RaftMessageQue;
template<typename T>
class NodeServerServiceImpl final : public Raft::NodeServer::Service {
public:
    NodeServerServiceImpl(
            Raft::RaftStateMachine<T>* raft,
            RaftMessageQue* receiveQueue)
        : _raft(raft)
        , _receiveQueue(receiveQueue)
    {}
    ~NodeServerServiceImpl() {}
    ::grpc::Status Raft(::grpc::ServerContext* context,
                        const ::Raft::RaftMessage* request, ::Raft::Done* response) override
    {
        assert(false);
        return grpc::Status::OK;
    }
    ::grpc::Status Get(::grpc::ServerContext* context,
                       const ::Raft::PutRequest* request, ::Raft::Done* response) override
    {
        assert(false);
        return grpc::Status::OK;
    }
    ::grpc::Status Add(::grpc::ServerContext* context,
                       const ::Raft::PutRequest* request, ::Raft::Done* response) override
    {
        assert(false);
        return grpc::Status::OK;
    }

private:
    Raft::RaftStateMachine<T>* _raft;
    RaftMessageQue* _receiveQueue;
};

#endif //RAFT_RAWNODE_H
