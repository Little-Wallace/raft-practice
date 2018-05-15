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
    virtual int64_t GetLastTerm() const = 0;
    virtual int64_t GetCommitted() const = 0;
    virtual int64_t GetTerm(int64_t idx) const = 0;
    virtual int64_t GetFirstIndex() const = 0;
    virtual int64_t GetLastIndex() const = 0;
    virtual int64_t GetApplied() const = 0;
    virtual bool MatchTerm(int64_t idx, int64_t term) const = 0;
    virtual void CommitTo(int64_t index) = 0;
    virtual void ApplyTo(int64_t index) = 0;
    virtual void StableTo(int64_t index) = 0;
    virtual int64_t FindConflict() = 0;
    virtual google::protobuf::RepeatedField<Entry> GetEntries(int64_t idx, uint64_t max_size) const = 0;
    virtual google::protobuf::RepeatedField<Entry> GetEntries() = 0;
    virtual bool MaybeCommit(uint16_t idx, uint16_t term) = 0;
    virtual int64_t MaybeAppend(uint16_t idx, uint16_t term, uint64_t commited,
                                const google::protobuf::RepeatedPtrField<Entry>& ents) const = 0;
    virtual int64_t Append(const google::protobuf::RepeatedPtrField<Entry>& ents) const = 0;
public:
    const static uint64_t NO_LIMIT = std::numeric_limits<uint64_t>::max();
};

};


#endif //RAFT_RAFT_LOG_H
