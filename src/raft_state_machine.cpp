#include "raft_state_machine.h"
using namespace std;

namespace Raft{

RaftStateMachine::RaftStateMachine(uint64_t id_,
                                   RaftLog* log,
                                   const std::vector<RaftNode*>& nodes)
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
    _votes.resize(nodes.size(), false);
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

bool RaftStateMachine::Step(const RaftMessage& msg) {
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
        MaybeCompaign();
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
}

void RaftStateMachine::DoCompaign() {
    // todo: judge if there is a entry of config change
    BecomeCandidate();
    if (Quorum() == Poll(_id, MsgRequestVote, true)) {
        BecomeLeader();
        return;
    }
    LaunchVote(MsgRequestVote);
}


void RaftStateMachine::MaybeCompaign() {
    // todo: judge if there is a entry of config change
    BecomePreCandidate();
    if (Quorum() == Poll(_id, MsgRequestPreVote, true)) {
        DoCompaign();
        return;
    }
    LaunchVote(MsgRequestPreVote);
}

void RaftStateMachine::LaunchVote(MessageType type) {
    uint64_t term = (type == MsgRequestPreVote ? (1 + _term) : _term);
    for (size_t i = 0; i < _nodes.size(); i ++)
    {
        auto m = CreateRaftMessage(type, term, _nodes[i]->GetId());
        m->set_index(_log->GetLastIndex());
        m->set_log_term(_log->GetLastTerm());
        Send(*m);
    }
}

void RaftStateMachine::DoVote(const RaftMessage& msg) {
    assert(false);
}
void RaftStateMachine::StepLeader(const RaftMessage& msg) {
    assert(false);
}
void RaftStateMachine::StepFollower(const RaftMessage& msg) {
    assert(false);
}

void RaftStateMachine::StepCandidate(const RaftMessage& msg) {
    assert(false);
}

void RaftStateMachine::BecomeFollower(uint64_t term, uint64_t id) {
    Reset(term);
    _leader_id = id;
    _state = Follower; 
}

void RaftStateMachine::BecomeCandidate() {
    assert(false);
}
void RaftStateMachine::BecomeLeader() {
    assert(false);
}
void RaftStateMachine::BecomePreCandidate() {
    assert(false);
}

};
