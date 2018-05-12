//
// Created by liuwei on 5/12/18.
//
#include "raft_state_machine.h"

using namespace std;
using namespace google::protobuf;

namespace Raft{

bool RaftStateMachine::StepLeader(RaftMessage& msg) {
    MessageType type = msg.msg_type();
    switch (type) {
        case MsgBeat:
            assert(false);
            //todo: broadcast heartbeat to follower
            break;
        case MsgCheckQuorum:
            assert(false);
            break;
        case MsgPropose:
            assert(false);
            break;
        case MsgReadIndex:
            assert(false);
            break;
        case MsgAppendResponse:
            HandleAppendResponse(msg);
            break;
        case MsgHeartbeatResponse:
            HandleHeartBeatResponse(msg);
            break;
        case MsgSnapStatus:
            if (GetRaftNode(msg.from())->GetState() == RaftNode::Snapshot)
            HandleSnapStatus(msg);
            break;
        case MsgUnreachable:
            // todo: ???????????
            assert(false);
            break;
        case MsgTransferLeader:
            HandleTransferLeader(msg);
            break;
        default:
            break;
    }
}

void RaftStateMachine::AppendEntry(RepeatedPtrField<Entry>& entries) {

}
void RaftStateMachine::BecomeLeader() {
    Reset(_term);
    _leader_id = _id;
    auto ents = _log->GetEntries(_log->GetCommitted() + 1, RaftLog::NO_LIMIT);
    // todo: change conf
    _state = Leader;
    RepeatedPtrField<Entry> vec;
    AppendEntry(vec);
}

void RaftStateMachine::HandleHeartBeatResponse(RaftMessage& msg) {
    RaftNode* node = GetRaftNode(msg.from());
    node->SetRecentActive(true);
    node->Resume();
    if (node->GetState() == RaftNode::Replicate && node->IsFull()) {
        // why free?
        //todo: node->FreeFirstOne();
    }
    if (node->GetMatched() < _log->GetLastIndex()) {
        // todo: send append
    }
    if (msg.context().empty()) {
        return;
    }
    /* if (_read_only->recv_ack(msg) < Quorum()) {
        return;
    } */
    //  _read_only->Advance(msg)


}
void RaftStateMachine::HandleTransferLeader(RaftMessage& msg) {

}

};

