//
// Created by liuwei on 4/29/18.
//

#include "raft_node.h"
using namespace std;

RaftNode::RaftNode(size_t id_, const std::string& address_)
    : _id(id_)
    , _address(address_)
{
}

bool RaftNode::Init()
{
    return _client.Init(_address);
}

