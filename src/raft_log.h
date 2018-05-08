//
// Created by liuwei on 5/1/18.
//

#ifndef RAFT_LOG_H
#define RAFT_LOG_H
#include "common.h"
#include <limits>

namespace Raft{

class RaftLog {
public:
    RaftLog() {}
    virtual ~RaftLog() {}
    virtual uint64_t GetLastIndex() const = 0;
    virtual uint64_t GetLastTerm() const = 0;
    virtual uint64_t GetCommitted() const = 0;
    virtual std::vector<Entry> Entries(uint64_t idx, uint64_t max_size) = 0;

public:
    const static uint64_t NO_LIMIT = std::numeric_limits<uint64_t>::max();
};

};


#endif //RAFT_RAFT_LOG_H
