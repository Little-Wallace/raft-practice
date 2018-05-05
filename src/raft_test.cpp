#include <gtest/gtest.h>
#include <iostream>
#include "service_keeper.h"
#include "raft_state_machine.h"

using namespace Raft;
using namespace std;

class GlobalTest : public testing::Test
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
};

TEST_F(GlobalTest, TestEnvironment)
{
    cout << "test success!" << endl;
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    //testing::Environment* env = new GlobalTest();
    //testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}


