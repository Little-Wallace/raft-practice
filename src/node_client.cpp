//
// Created by liuwei on 4/23/18.
//

#include "node_client.h"
#include <grpc++/grpc++.h>
#include <grpc/grpc.h>

using namespace Node;
using namespace std;

bool NodeClient::Init(const std::string& address)
{
    _stub = NodeServer::NewStub(grpc::CreateChannel(address,
                                                    grpc::InsecureChannelCredentials()));
    return _stub ? true: false;
}

bool NodeClient::AddLog(const std::string& content)
{

    grpc::ClientContext context;
    Node::Done done;
    PutRequest request;
    request.set_content(content);
    auto ret = _stub->AddLog(&context, request, &done);
    return ret.ok();
}
