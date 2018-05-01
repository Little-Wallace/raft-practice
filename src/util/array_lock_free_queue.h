#ifndef __ARRAY_LOCL_FREE_QUEUE
#define __ARRAY_LOCL_FREE_QUEUE
#include <atomic>
#include <unistd.h>

template<typename T, uint32_t Q_SIZE>
class ArrayLockFreeQueue
{
public:
    ArrayLockFreeQueue()
        : _head(0)
        , _write_tail(0)
        , _read_tail(0)
    {}
    ~ArrayLockFreeQueue() {
    }
public:
    bool Push(const T& term);
    bool Pop(T& pop);
private:
    uint32_t HashIndex(uint32_t offset);
    T _data[Q_SIZE];
    std::atomic<uint32_t> _head;
    std::atomic<uint32_t> _write_tail;
    std::atomic<uint32_t> _read_tail;
};

template<typename T, uint32_t Q_SIZE>
inline uint32_t ArrayLockFreeQueue<T, Q_SIZE>::HashIndex(uint32_t offset)
{
    return offset % Q_SIZE;
}

template<typename T, uint32_t Q_SIZE>
bool ArrayLockFreeQueue<T, Q_SIZE>::Push(const T& term)
{
    uint32_t tail = 0;
    do
    {
        tail = _write_tail.load();
        if (HashIndex(tail + 1) == HashIndex(_head.load()))
        {
            return false;
        }
    } while(!_write_tail.compare_exchange_weak(tail, tail + 1));
    _data[HashIndex(tail)] = term;
    while(!_read_tail.compare_exchange_weak(tail, tail + 1))
    {
        usleep(10);
    }
    return true;
}

template<typename T, uint32_t Q_SIZE>
bool ArrayLockFreeQueue<T, Q_SIZE>::Pop(T& term)
{
    uint32_t head = 0;
    do
    {
        head = _head.load();
        if (HashIndex(head) == HashIndex(_read_tail.load()))
        {
            return false;
        }
        term = _data[HashIndex(head)];
        if (_head.compare_exchange_weak(head, head + 1))
        {
            return true;
        }
    } while(1);
    assert(false);
    return false;
}

#endif

