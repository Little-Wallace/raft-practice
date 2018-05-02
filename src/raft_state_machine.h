//
// Created by liuwei on 5/1/18.
//

#ifndef RAFT_STATE_MACHINE_H
#define RAFT_STATE_MACHINE_H
#include "common.h"
#include "raft_node.h"
#include "util/array_lock_free_queue.h"

#include <random>

namespace Raft{

RaftMessage* NewRaftMessage(MessageType msg_type,
                           uint64_t to,
                           uint64_t from)
{
    RaftMessage* msg = new RaftMessage();
    msg->set_term(0);
    msg->set_msg_type(msg_type);
    msg->set_to(to);
    msg->set_from(from);
    return msg;
}

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

template<typename T>
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
    typedef T RaftLog;
    RaftStateMachine(uint64_t id_,
                     RaftLog* log,
                     const std::vector<RaftNode>& nodes);
    
    ~RaftStateMachine() {}
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
    
private:
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
    std::vector<RaftMessage> _msgs;
    RaftLog* _log;
    std::vector<RaftNode> _nodes;
    std::vector<RaftMessage> _to_send_msgs;
};

template<typename T>
RaftStateMachine<T>::RaftStateMachine(uint64_t id_,
                                      RaftLog* log,
                                      const std::vector<RaftNode>& nodes)
    : _id(id_)
    , _leader_id(0)
    , _vote(0)
    , _term(0)
    , _election_elapsed(0)
    , _heartbeat_elapsed(0)
    , _election_timeout(DEFAULT_ELECTION_TIMEOUT)
    , _heartbeat_timeout(DEFAULT_HEARTBEAT_TIMEOUT)
    , _log(log)
    , _nodes(nodes) {
}

template<typename T>
void RaftStateMachine<T>::BecomeFollower(uint64_t term, uint64_t id)
{
    Reset(term);
    _leader_id = id;
    _state = Follower; 
}

template<typename T>
void RaftStateMachine<T>::Reset(uint64_t term)
{
    if (_term != term) {
        _term = term;
        _vote = INVALID_ID;
    }
    _leader_id = INVALID_ID;
    ResetRandomizedElectionTimeout();
    _election_elapsed = 0;
    _heartbeat_elapsed = 0;
    
    // _abort_leader_transfer();
    
    // _votes = FxHashMap::default();
    
    //_pending_conf_index = 0;
    //_read_only = ReadOnly::new(_read_only.option);

    // let (last_index, max_inflight) = (_log.last_index(), _max_inflight);
    // let self_id = _id;
    // for (size_t i = 0; i < _nodes.size(); i ++)
    // {
        
    // }
    // for (&id, pr) in _mut_prs().iter_mut() {
    //         let is_learner = pr.is_learner;
    //         *pr = new_progress(last_index + 1, max_inflight);
    //         pr.is_learner = is_learner;
    //         if id == self_id {
    //             pr.matched = last_index;
    //         }
    //     }
    // }
}

template<typename T>
bool RaftStateMachine<T>::Step(const RaftMessage& msg) {
    if (msg.term() == 0) {
        // local message
    } else if (msg.term() > _term) {
        // remote new message
        assert(false);
    } else if (msg.term() < _term) {
        // remote old message, maybe compaign again.
        assert(false);
    }
    MessageType msg_type = msg.msg_type();
    switch (msg_type) {
    case MsgHup:
        // regular tick to drive raft
        assert(false);
        break;
    case MsgRequestPreVote:
    case MsgRequestVote:
        // compaign
        assert(false);
        break;
    default:
        assert(false);
        break;
    }
}

}
#endif

