//
// Created by liuwei on 5/12/18.
//
#include "raft_state_machine.h"

using namespace std;

namespace Raft{

bool RaftStateMachine::StepCandidate(RaftMessage& msg) {
    cout << "step candidate: " << msg.msg_type() << " " << MsgRequestPreVoteResponse << endl;
    switch (msg.msg_type()) {
        case MsgPropose:
            return false;
        case MsgAppend:
            BecomeFollower(msg.term(), msg.from());
            HandleAppendEntries(msg);
        break;
        case MsgHeartbeat:
            BecomeFollower(msg.term(), msg.from());
            HandleHeartBeat(msg);
            break;
        case MsgSnapshot:
            BecomeFollower(msg.term(), msg.from());
            HandleSnapshot(msg);
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
                    DoCampaign();
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

void RaftStateMachine::DoCampaign() {
    // todo: judge if there is a entry of config change
    cout << "begin campagin" << endl;
    BecomeCandidate();
    if (Quorum() == Poll(_id, MsgRequestVote, true)) {
        BecomeLeader();
        return;
    }
    LaunchVote(MsgRequestVote);
}

void RaftStateMachine::MaybeCampaign() {
    // todo: judge if there is a entry of config change
    cout << "begin pre campagin" << endl;
    BecomePreCandidate();
    if (Quorum() == Poll(_id, MsgRequestPreVote, true)) {
        DoCampaign();
        return;
    }
    LaunchVote(MsgRequestPreVote);
}

void RaftStateMachine::BecomeCandidate() {
    Reset(_term + 1);
    _vote = _id;
    _state = Candidate;
}

void RaftStateMachine::BecomePreCandidate() {
    _state = PreCandidate;
}

};
