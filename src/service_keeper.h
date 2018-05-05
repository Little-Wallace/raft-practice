//
// Created by liuwei on 4/23/18.
//

#ifndef SERVICE_KEEPER_H
#define SERVICE_KEEPER_H

#include <string>
#include <memory>

#include "message.grpc.pb.h"
#include "raft_state_machine.h"

class ServiceKeeper {
public:
    typedef ::ArrayLockFreeQueue<Raft::RaftMessage, 100> RaftMessageQueue;
    ServiceKeeper(Raft::RaftStateMachine* raft,
                  RaftMessageQueue* que)
        : _run(false)
        , _raft(raft)
        , _receive_que(que)
    {}
    ~ServiceKeeper() {}
    bool Start(int64_t interval);
    void Stop();
private:
    ServiceKeeper(const ServiceKeeper& other);
    ServiceKeeper& operator=(const ServiceKeeper& other);
    void WorkLoop(int64_t tick_interval);
    void Tick() {
        _raft->Tick();
    }
    void OnRaftReady()
    {
        assert(false);
    }
private:
    volatile bool _run;
    std::thread* _work_loop;
    Raft::RaftStateMachine* _raft;
    RaftMessageQueue* _receive_que;
};


#endif //SERVICE_KEEPER_H
