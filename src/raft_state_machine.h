//
// Created by liuwei on 5/1/18.
//

#ifndef RAFT_STATE_MACHINE_H
#define RAFT_STATE_MACHINE_H
#include "raft_node.h"
#include "raft_log.h"
#include "util/array_lock_free_queue.h"
#include <random>
#include "common.h"

namespace Raft{
// RaftMessage NewRaftMessage(MessageType msg_type,
//                            uint64_t to,
//                            uint64_t from)
// {
//     RaftMessage msg;
//     msg.set_msg_type(msg_type);
//     msg.set_to(to);
//     msg.set_from(from);
//     msg.set_term(0);
//     return msg;    
// }

class RaftStateMachine
{
public:
    enum RoleState
    {
        Follower,
        Candidate,
        Leader,
        PreCandidate
    };
    RaftStateMachine(uint64_t id_,
                     RaftLog* log,
                     const std::vector<RaftNode*>& nodes);
    
    ~RaftStateMachine();
    bool Step(const ::Raft::RaftMessage& msg);
    void Reset(uint64_t term);
    void ResetRandomizedElectionTimeout() {
        static std::default_random_engine rd;
        rd.seed(_id);
        uint64_t timeout = rd() % _election_timeout + _election_timeout;
        _randomized_election_timeout = timeout;
    }
    void BecomeFollower(uint64_t term, uint64_t id);
    bool GetSendMessages(std::vector<RaftMessage>& to_send_msgs);
    uint32_t Poll(uint64_t id_, MessageType type, bool agree);
    void Tick()
    {
        assert(false);
    }

    bool HasReady()
    {
        assert(false);
        return false;
    }
    
    static RaftMessage* CreateRaftMessage(MessageType msg_type,
            uint64_t term,
            uint64_t to)
    {
        RaftMessage* msg = new RaftMessage();
        msg->set_term(term);
        msg->set_msg_type(msg_type);
        msg->set_to(to);
        return msg;
    }
private:
    void Send(RaftMessage& msg);
    void MaybeCompaign();
    uint32_t Quorum();
    void DoCompaign();
    void DoVote(const RaftMessage& msg);
    void StepLeader(const RaftMessage& msg);
    void StepFollower(const RaftMessage& msg);
    bool StepCandidate(const RaftMessage& msg);
    void BecomeCandidate();
    void BecomeLeader();
    void BecomePreCandidate();
    void LaunchVote(MessageType type);
    void AppendEntry(std::vector<Entry>& entries) {
        assert(false);
    }
    void BroadCast() {
        assert(false);
    }
private :
    uint64_t _id;
    uint64_t _leader_id;
    uint64_t _vote;
    uint64_t _term;
    uint64_t _election_elapsed;
    uint64_t _heartbeat_elapsed;
    uint64_t _election_timeout;
    uint64_t _heartbeat_timeout;
    uint64_t _randomized_election_timeout;
    RoleState _state;
    RaftLog* _log;
    std::vector<RaftNode*> _nodes;
    std::map<uint64_t, bool> _votes;
    std::vector<RaftMessage> _to_send_msgs;
    // todo: use allocator to avoid allocate memory for messages frequently
    // Allocator<RaftMessage> _allocator;

    friend class RaftTest_TestCampaign_Test;
};

inline uint32_t RaftStateMachine::Quorum()
{
    return _nodes.size() / 2 + 1;
}

// todo: use message type for log
inline uint32_t RaftStateMachine::Poll(uint64_t id_, MessageType type, bool agree) {
    _votes[id_] = agree;
    uint32_t vote_result = 0;
    for (auto iter: _votes) {
        if (iter.second) vote_result ++;
    }
    return vote_result;
}

inline void RaftStateMachine::Reset(uint64_t term) {
    if (_term != term) {
        _term = term;
        _vote = INVALID_ID;
    }
    _leader_id = INVALID_ID;
    ResetRandomizedElectionTimeout();
    _election_elapsed = 0;
    _heartbeat_elapsed = 0;
    _votes.clear();
    // reset nodes
}


};
#endif

