//
// Created by liuwei on 4/23/18.
//

#include "node_client.h"
#include <grpc++/grpc++.h>
#include <grpc/grpc.h>

using namespace Raft;
using namespace std;
using namespace grpc;

bool NodeClient::Init(const std::string& address)
{
    _timespec.tv_sec = 2;//设置阻塞时间为2秒
    _timespec.tv_nsec = 0;
    _timespec.clock_type = GPR_TIMESPAN;
    _stub = NodeServer::NewStub(grpc::CreateChannel(address,
                                                    grpc::InsecureChannelCredentials()));
    return _stub ? true: false;
}

bool NodeClient::Add(const std::string& content)
{
    grpc::ClientContext context;
    context.set_deadline(_timespec);
    Done done;
    PutRequest request;
    request.set_content(content);
    auto ret = _stub->Add(&context, request, &done);
    return ret.ok();
}

NodeClient::ResStatus NodeClient::Send(const Raft::RaftMessage& message)
{
    grpc::ClientContext context;
    context.set_deadline(_timespec);
    Done done;
    auto ret = _stub->Raft(&context, message, &done);
    if (ret.ok()) {
        return OK;
    } else if (ret.error_code() == grpc::DEADLINE_EXCEEDED) {
        return TIMEOUT;
    } else {
        return ERROR;
    }
}
