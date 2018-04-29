//
// Created by liuwei on 4/23/18.
//

#include "node_server_service_impl.h"
#include <unistd.h>
#include <iostream>
#include <memory>
#include <grpc/grpc.h>
#include <grpc++/grpc++.h>

using namespace std;
using namespace Node;
using grpc::Status;

NodeServerServiceImpl::NodeServerServiceImpl(
            const string& address,
            const vector<string>& clusters)
        : _run(false)
{
    for (size_t i = 0; i < clusters.size(); i ++)
    {
        if (clusters[i] == address) {
            _id = i;
            _run = true;
        }
    }
}

NodeServerServiceImpl::~NodeServerServiceImpl()
{
    _run = false;
}



