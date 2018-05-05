//
// Created by liuwei on 5/1/18.
//

#ifndef RAFT_COMMON_H
#define RAFT_COMMON_H
#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <vector>
#include "message.pb.h"

static const uint64_t DEFAULT_ELECTION_TIMEOUT = 2000000;
static const uint64_t DEFAULT_HEARTBEAT_TIMEOUT = 2000000;
static const uint64_t DEFAULT_REQUEST_TIMEOUT = 1;
static const uint64_t INVALID_ID = 0;
#endif //RAFT_COMMON_H
