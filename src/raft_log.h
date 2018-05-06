//
// Created by liuwei on 5/1/18.
//

#ifndef RAFT_LOG_H
#define RAFT_LOG_H
#include "common.h"

namespace Raft{

class RaftLog {
public:
    RaftLog() {}
    virtual ~RaftLog() {}
    virtual uint64_t GetLastIndex() const = 0;
    virtual uint64_t GetLastTerm() const = 0;
};

};


#endif //RAFT_RAFT_LOG_H
