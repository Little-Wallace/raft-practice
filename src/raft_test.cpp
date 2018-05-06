#define private public
#define protected public
#include <gtest/gtest.h>
#include <iostream>
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
};

class RaftTest : public testing::Test
{
public:
    virtual void SetUp()
    {
        cout << "==========Begin Test!================" << endl; 
    }
    virtual void TearDown()
    {
        cout << "==========End Test!==================" << endl;
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


TEST_F(RaftTest, TestCampaign)
{
    cout << "create nodes" << endl;
    auto nodes = CreateRaftNodes();
    RaftStateMachine raft(0, new MockRaftLog, nodes);
    raft.BecomeFollower(0, 0);
    cout << "befor step" << endl;
    RaftMessage* msg = RaftStateMachine::CreateRaftMessage(MsgHup, 0, 0);
    cout << "befor step" << endl;
    raft.Step(*msg);
    cout << "end step" << endl;
    ASSERT_EQ(RaftStateMachine::PreCandidate, raft._state);
    ASSERT_EQ(nodes.size() - 1, raft._to_send_msgs.size());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    //testing::Environment* env = new GlobalTest();
    //testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}


