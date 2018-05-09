#include "raft_state_machine.h"
#include <exception>
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
    return true;
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
        if (_nodes[i]->GetId() == _id) continue;
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

bool RaftStateMachine::StepCandidate(const RaftMessage& msg) {
    cout << "step candidate: " << msg.msg_type() << " " << MsgRequestPreVoteResponse << endl; 
    switch (msg.msg_type()) {
    case MsgPropose:
        return false;
    case MsgAppend:
    case MsgHeartbeat:
    case MsgSnapshot:
        BecomeFollower(msg.term(), msg.from());
        assert(false);
        // todo: handle msg
        break;
    case MsgRequestPreVoteResponse:
    case MsgRequestVoteResponse:
        cout << "step " << _state << ": " << PreCandidate << endl;
        if ((MsgRequestPreVoteResponse == msg.msg_type() && PreCandidate != _state)
            || (MsgRequestVoteResponse == msg.msg_type() && Candidate != _state))
            return true;
        {
            size_t sz = Poll(msg.from(), msg.msg_type(), !msg.reject());
            if (Quorum() == sz) {
                // win the campaign, become leader immediately
                if (PreCandidate == _state) {
                    DoCompaign();
                } else{
                    BecomeLeader();
                    BroadCast();
                }
            } else if (_votes.size() == sz + Quorum()) {
                // lose the campaign, wait someone to be leader.
                BecomeFollower(_term, INVALID_ID);
            }
        }
        break;
        case MsgTimeoutNow:
            cerr << "debug time out now, id " << _id << " term: " << _term << " state: " << _state << endl;
            break;
        default:
            assert(false);
            break;
    }
    return true;
}

void RaftStateMachine::BecomeFollower(uint64_t term, uint64_t id) {
    Reset(term);
    _leader_id = id;
    _state = Follower; 
}

void RaftStateMachine::BecomeCandidate() {
    Reset(_term + 1);
    _vote = _id;
    _state = Candidate;
}
void RaftStateMachine::BecomeLeader() {
    Reset(_term);
    _leader_id = _id;
    auto ents = _log->Entries(_log->GetCommitted() + 1, RaftLog::NO_LIMIT);
    // todo: change conf
    _state = Leader;
    vector<Entry> vec;
    AppendEntry(vec);
}
void RaftStateMachine::BecomePreCandidate() {
    _state = PreCandidate;
}

bool RaftStateMachine::GetSendMessages(std::vector<RaftMessage>& to_send_msgs) {
    to_send_msgs.swap(_to_send_msgs);
}

};
