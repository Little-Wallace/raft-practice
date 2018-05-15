#include "raft_state_machine.h"
#include <exception>
using namespace std;

namespace Raft{

RaftStateMachine::RaftStateMachine(uint64_t id_,
                                   RaftLog* log,
                                   const std::map<int64_t, RaftNode*>& nodes)
    : _id(id_)
    , _leader_id(0)
    , _leader_transferee(0)
    , _vote(0)
    , _term(0)
    , _election_elapsed(0)
    , _heartbeat_elapsed(0)
    , _election_timeout(DEFAULT_ELECTION_TIMEOUT)
    , _heartbeat_timeout(DEFAULT_HEARTBEAT_TIMEOUT)
    , _log(log)
    , _nodes(nodes) {
}

RaftStateMachine::~RaftStateMachine() {
}


void RaftStateMachine::Send(RaftMessage& msg) {
    msg.set_from(_id);
    auto type = msg.msg_type();
    if (MsgRequestVote == type ||
        MsgRequestPreVote == type ||
        MsgRequestVoteResponse == type ||
        MsgRequestPreVoteResponse == type) {
        // term_id must be set to decide which node should win the campaign
        assert(msg.term());
    } else {
        assert(0 == msg.term());
        // follower ask for following leader.
        if (MsgPropose != type ||
            MsgReadIndex != type) {
            msg.set_term(_term);
        }
    }
    _to_send_msgs.push_back(msg);
}

bool RaftStateMachine::Step(RaftMessage& msg) {
    MessageType msg_type = msg.msg_type();
    if (msg.term() == 0) {
        // local message
    } else if (msg.term() > _term) {
        if (MsgRequestVote == msg_type || MsgRequestPreVote == msg_type) {
            bool force = msg.context() == CAMPAIGN_TRANSFER;
            bool in_lease = _leader_id != INVALID_ID && _election_elapsed < _election_timeout;
            if (!force && in_lease) {
                return true;
            }
        }
        if ((MsgRequestPreVoteResponse == msg_type && !msg.reject())
            || MsgRequestPreVote == msg_type) {
            // prevote
        } else {
            // receive a higher term, should be follower
            if (MsgAppend == msg_type ||
                    MsgHeartbeat == msg_type ||
                    MsgSnapshot == msg_type) {
                BecomeFollower(msg.term(), msg.from());
            } else {
                BecomeFollower(msg.term(), INVALID_ID);
            }
        }
    } else if (msg.term() < _term) {
        // remote old message, maybe compaign again.
        // receive msg from a obsolete leader.
        if (MsgHeartbeat == msg_type || MsgAppend == msg_type) {
            RaftMessage* msg = CreateRaftMessage(MsgAppendResponse, 0, 0);
            Send(*msg);
        }
        assert(false);
    }
    switch (msg_type) {
    case MsgHup:
        // campaign
        MaybeCampaign();
        break;
    case MsgRequestPreVote:
    case MsgRequestVote:
        // vote for compaign
        DoVote(msg);
        break;
    default:
        if (_state == PreCandidate || _state == Candidate) {
            StepCandidate(msg);
        } else if (_state == Follower) {
            StepFollower(msg);
        } else if (_state == Leader) {
            StepLeader(msg);
        } else {
            assert(false);
        }
        break;
    }
    return true;
}


void RaftStateMachine::LaunchVote(MessageType type) {
    uint64_t term = (type == MsgRequestPreVote ? (1 + _term) : _term);
    for (auto ite: _nodes) {
        if (ite.second->GetId() == _id) continue;
        auto m = CreateRaftMessage(type, term, ite.second->GetId());
        m->set_index(_log->GetLastIndex());
        m->set_log_term(_log->GetLastTerm());
        Send(*m);
    }
}

void RaftStateMachine::DoVote(const RaftMessage& msg) {
    assert(false);
}

void RaftStateMachine::StepFollower(const RaftMessage& msg) {
    assert(false);
}

bool RaftStateMachine::GetSendMessages(std::vector<RaftMessage>& to_send_msgs) {
    to_send_msgs.swap(_to_send_msgs);
}

void RaftStateMachine::BecomeFollower(uint64_t term, uint64_t id) {
    Reset(term);
    _leader_id = id;
    _state = Follower;
}

void RaftStateMachine::HandleAppendEntries(const RaftMessage& msg) {
    // local message is more fresh, do not change
    RaftMessage* to_send = CreateRaftMessage(MsgAppendResponse, 0, msg.from());
    if (msg.index() < _log->GetCommitted()) {
        to_send->set_index(_log->GetCommitted());
        Send(*to_send);
        return;
    }

    int64_t last_index = _log->MaybeAppend(msg.index(), msg.log_term(),
            msg.commit(), msg.entries());
    if (~last_index) {
        to_send->set_index(last_index);
    } else {
        // reject message
        to_send->set_index(msg.index());
        to_send->set_reject(true);
        to_send->set_reject_hint(_log->GetLastIndex());
    }
    Send(*to_send);
    
}

void RaftStateMachine::HandleHeartBeat(RaftMessage& msg) {
    _log->CommitTo(msg.commit());
    RaftMessage* m = CreateRaftMessage(MsgHeartbeatResponse, 0, msg.from());
    m->set_allocated_context(msg.release_context());
    Send(*m);
}

void RaftStateMachine::HandleSnapshot(RaftMessage& msg) {
    // todo: move region data
    assert(false);

}

};
