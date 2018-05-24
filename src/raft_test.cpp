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
    int64_t GetLastIndex() const override {
        return 1;
    }
    int64_t GetLastTerm() const override {
        return 1;
    }
    int64_t GetCommitted() const override {
        return 1;
    }
    int64_t GetTerm(int64_t idx) const override {
        return 1;
    }
    int64_t GetFirstIndex() const override {
        return 1;
    }
    int64_t GetApplied() const override {
        return 1;
    }
    bool MatchTerm(int64_t idx, int64_t term) const override {
        return false;
    }

    void CommitTo(int64_t index) override {
    }
    void ApplyTo(int64_t index) override {
    }
    void StableTo(int64_t index) override {
    }
    int64_t FindConflict() override {
        return 1;
    }
    google::protobuf::RepeatedField<Entry> GetEntries(int64_t idx, uint64_t max_size) const override {
        return google::protobuf::RepeatedField<Entry> ();
    }
    google::protobuf::RepeatedField<Entry> GetEntries() override {
        return google::protobuf::RepeatedField<Entry> ();
    }
    bool MaybeCommit(uint16_t idx, uint16_t term) override {
        return false;
    }
    int64_t MaybeAppend(uint16_t idx, uint16_t term, uint64_t commited,
                                const google::protobuf::RepeatedPtrField<Entry>& ents) const override {
        return 1;
    }
    int64_t Append(const google::protobuf::RepeatedPtrField<Entry>& ents) const override {
        return 1;
    }

};

class RaftTest : public testing::Test
{
public:
    void SetUp()
    {
        cerr << "==========Begin Test!================" << endl;
    }
    void TearDown()
    {
        cerr << "==========End Test!==================" << endl;
    }
protected:
    map<int64_t, RaftNode*> CreateRaftNodes(size_t cluster_size = 3) {
        map<int64_t, RaftNode*> nodes;
        for (int64_t i = 1; i <= cluster_size; i ++) {
            nodes[i] = new RaftNode(i, "");
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
    RaftMessage* msg = RaftStateMachine::CreateRaftMessage(MsgHup, 0, 1);
    cout << "befor step" << endl;
    raft.Step(*msg);
    cout << "end step" << endl;
    ASSERT_EQ(RaftStateMachine::PreCandidate, raft._state);
    ASSERT_EQ(2, raft._to_send_msgs.size());
    vector<RaftMessage*> msgs;
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
    cout << "end pre vote" << endl;
    for (size_t i = 2; i <= 3; i ++) {
        msg->set_msg_type(MsgRequestVoteResponse);
        msg->set_from(i);
        msg->set_reject(false);
        raft.Step(*msg);
    }
    ASSERT_EQ(RaftStateMachine::Leader, raft._state);
}



int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    //testing::Environment* env = new GlobalTest();
    //testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}


