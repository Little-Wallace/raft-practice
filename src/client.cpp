//
// Created by liuwei on 4/29/18.
//

#include <iostream>
#include <gflags/gflags.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include "node_client.h"

using namespace std;
using namespace Node;
using grpc::Server;
using grpc::ServerBuilder;

DEFINE_string(address, "127.0.0.1:8000", "the server address");

int main(int argc, char** argv) {
    NodeClient client;
    if (!client.Init(FLAGS_address)) {
        cout << "init failed" << endl;
        return 0;
    }
    string content;
    while (cin >> content) {
        bool ret = client.Add(content);
        if (ret) {
            cout << "send success!" << endl;
        } else {
            cout << "send fail !" << endl;
        }
    }
}

