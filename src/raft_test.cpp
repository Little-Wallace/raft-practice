#include <gtest/gtest.h>
#include "common.h"
#include <iostream>
#define private public
#define protected public

#include "service_keeper.h"

#include "raft_state_machine.h"
#include "raft_log.h"


using namespace Raft;
using namespace std;

class MockRaftLog : public RaftLog
{
public:
    uint64_t GetLastIndex() const override {
        return 1;
    }
    uint64_t GetLastTerm() const override {
        return 1;
    }
    uint64_t GetCommitted() const override {
        return 1;
    }
    vector<Raft::Entry> Entries(uint64_t idx, uint64_t max_size) override {
        vector<Raft::Entry> ents;
        return ents;
    }

};

class RaftTest : public testing::Test
{
public:
    virtual void SetUp()
    {
        cerr << "==========Begin Test!================" << endl;
    }
    virtual void TearDown()
    {
        cerr << "==========End Test!==================" << endl;
    }
protected:
    vector<RaftNode*> CreateRaftNodes(size_t cluster_size = 3) {
        vector<RaftNode*> nodes;
        for (size_t i = 0; i < cluster_size; i ++) {
            nodes.push_back(new RaftNode(i + 1, ""));
        }
        return nodes;
    }
};


TEST_F(RaftTest, TestLaunchCampaignFail)
{
    cout << "create nodes" << endl;
    auto nodes = CreateRaftNodes();
    RaftStateMachine raft(1, new MockRaftLog, nodes);
    raft.BecomeFollower(0, 0);
    cout << "befor step" << endl;
    RaftMessage* msg = RaftStateMachine::CreateRaftMessage(MsgHup, 0, 1);
    cout << "befor step" << endl;
    raft.Step(*msg);
    cout << "end step" << endl;
    ASSERT_EQ(RaftStateMachine::PreCandidate, raft._state);
    ASSERT_EQ(2, raft._to_send_msgs.size());
    vector<RaftMessage> msgs;
    raft.GetSendMessages(msgs);
    ASSERT_EQ(0, raft._to_send_msgs.size());
    ASSERT_EQ(2, raft.Quorum());
    ASSERT_EQ(1, raft.Poll(1, MsgHup, true));
    for (size_t i = 2; i <= 3; i ++) {
        msg->set_msg_type(MsgRequestPreVoteResponse);
        msg->set_from(i);
        msg->set_reject(false);
        raft.Step(*msg);
    }
    ASSERT_EQ(RaftStateMachine::Candidate, raft._state);
    
}



int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    //testing::Environment* env = new GlobalTest();
    //testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}


