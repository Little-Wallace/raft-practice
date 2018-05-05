#include "service_keeper.h"

using namespace std;
using namespace Raft;

bool ServiceKeeper::Start(int64_t tick_interval)
{
    _run = true;
    _work_loop = new thread(&ServiceKeeper::WorkLoop, this, tick_interval);
    if (!_work_loop)
    {
        _run = false;
        return false;
    }
    return true;
}

void ServiceKeeper::WorkLoop(int64_t tick_interval)
{
    int64_t tick_request = 0;
    while (_run)
    {
        RaftMessage msg;
        if (_receive_que->Pop(msg))
        {
            _raft->Step(msg);
        }
        else
        {
            usleep(DEFAULT_REQUEST_TIMEOUT * 1000);
            if (tick_request > tick_interval)
            {
                tick_request = 0;
                Tick();
            }
        }
        if (_raft->HasReady())
        {
            OnRaftReady();
            continue;
        }
        tick_request += DEFAULT_REQUEST_TIMEOUT;
        //this_thread::sleep_for(DEFAULT_REQUEST_TIMEOUT);
    }
}

