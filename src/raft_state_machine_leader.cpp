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
            BroadCastHeartBeat();
            //todo: broadcast heartbeat to follower
            break;
        case MsgCheckQuorum:
            if (!CheckQuorum()) {
                BecomeFollower(0, INVALID_ID);
            }
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

// if more than one half node receive leader last message, commit it into log
bool RaftStateMachine::MaybeCommit() {
    int64_t matches[_nodes.size()];
    int idx = 0;
    for (auto ite: _nodes) {
       matches[idx ++] = ite.second->GetMatched();
    }
    sort(matches, matches + _nodes.size());
    return _log->MaybeCommit(matches[Quorum() - 1], _term);
}

void RaftStateMachine::AppendEntry(RepeatedPtrField<Entry>& entries) {
    int64_t sz = entries.size();
    int64_t idx = _log->GetLastIndex();
    for (size_t i = 0; i < sz; i ++) {
        entries[i].set_term(_term);
        entries[i].set_index(idx + i + 1);
    }
    idx = _log->Append(entries);
    GetRaftNode(_id)->MaybeUpdate(idx);
    MaybeCommit();
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
        assert(false);
    }
    if (node->GetMatched() < _log->GetLastIndex()) {
        // todo: send append
        assert(false);
    }
    if (msg.context().empty()) {
        return;
    }
    assert(false);
    /* if (_read_only->recv_ack(msg) < Quorum()) {
        return;
    } */
    //  _read_only->Advance(msg)


}
void RaftStateMachine::HandleTransferLeader(RaftMessage& msg) {

}

void RaftStateMachine::BroadCastHeartBeat() {
    assert(false);
}

bool RaftStateMachine::CheckQuorum() {
    size_t act = 0;
    for (auto node: _nodes) {
        if (node.second->GetId() == _id) {
            act += 1;
            continue;
        }
        if(node.second->GetRecentActive()) {
            act += 1;
        }
        node.second->SetRecentActive(false);
    }
    return act >= Quorum();
}

};

