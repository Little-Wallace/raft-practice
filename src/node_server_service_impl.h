//
// Created by liuwei on 4/23/18.
//

#ifndef RAFT_NODE_SERVER_H
#define RAFT_NODE_SERVER_H

#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include "message.grpc.pb.h"
#include "util/array_lock_free_queue.h"

namespace Raft
{
class RaftStateMachine;
};

typedef ::ArrayLockFreeQueue<Raft::RaftMessage, 100> RaftMessageQueue;

class NodeServerServiceImpl final : public Raft::NodeServer::Service {
public:
    NodeServerServiceImpl(
            Raft::RaftStateMachine* raft,
            RaftMessageQueue* receiveQueue);

    ~NodeServerServiceImpl();
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
    Raft::RaftStateMachine* _raft;
    RaftMessageQueue* _receiveQueue;
};

#endif //RAFT_NODE_SERVER_H
