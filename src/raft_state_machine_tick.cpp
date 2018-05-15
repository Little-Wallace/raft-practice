//
// Created by liuwei on 5/13/18.
//

#include "raft_state_machine.h"

using namespace std;
using namespace google::protobuf;

namespace Raft{

bool RaftStateMachine::Tick() {
    if (_state == Leader) {
        return TickHeartBeat();
    } else {
        // candidate, follower, precandidate
        return TickElection();
    }
}

bool RaftStateMachine::TickElection() {
    _election_elapsed += 1;
    if (_election_elapsed < _randomized_election_timeout) {
        return false;
    }
    _election_elapsed = 0;
    RaftMessage* msg = CreateRaftMessage(MsgHup, 0, INVALID_ID);
    msg->set_from(_id);
    Step(*msg);
    delete msg;
    return true;
}

bool RaftStateMachine::TickHeartBeat() {
    _heartbeat_elapsed += 1;
    _election_elapsed += 1;
    bool has_ready = false;
    if (_election_elapsed >= _election_timeout) {
        _election_elapsed = 0;
        RaftMessage* msg = CreateRaftMessage(MsgCheckQuorum, 0, INVALID_ID);
        msg->set_from(_id);
        Step(*msg);
        delete msg;
        has_ready = true;
    }
    if (Leader == _state && _leader_transferee) {
        _leader_transferee = 0; // todo: ?????
    }
    if (Leader != _state) {
        return has_ready;
    }
    // why he is stil leader?
    if (_heartbeat_elapsed >= _heartbeat_timeout) {
        _heartbeat_elapsed = 0;
        has_ready = true;
        RaftMessage* msg = CreateRaftMessage(MsgBeat, 0, INVALID_ID);
        msg->set_from(_id);
        Step(*msg);
        delete msg;
    }
    return has_ready;
}

};