#include "node_server_service_impl.h"
#include "raft_state_machine.h"

using namespace std;

NodeServerServiceImpl::NodeServerServiceImpl(
        Raft::RaftStateMachine* raft,
        RaftMessageQueue* receiveQueue)
    : _raft(raft)
    , _receiveQueue(receiveQueue)
{
}

NodeServerServiceImpl::~NodeServerServiceImpl()
{
}



