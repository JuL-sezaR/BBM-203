#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H

#include "Request.h"
#include <string>

class RequestQueue {
public:
    RequestQueue();
    explicit RequestQueue(int initialCapacity);
    ~RequestQueue();

    // Disable copying to keep ownership simple
    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator=(const RequestQueue&) = delete;

    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    bool getAt(int index, Request& out) const;

    // Enqueue at the rear (returns false on allocation failure)
    // TODO: Implement in RequestQueue.cpp.
    bool enqueue(const Request& req);
    void enqueueFront(const Request& req);

    // Dequeue from the front into 'outReq'.
    // Returns false if the queue is empty.
    // TODO: Implement in RequestQueue.cpp.
    bool dequeue(Request& outReq);

    // Peek the front element without removing.
    bool peek(Request& outReq) const;

    // Remove all elements from the queue (but keep allocated array).
    void clear();

    // Search for a request with given id and remove it if found.
    // Returns true if removed, false otherwise.
    // TODO: Implement in RequestQueue.cpp.
    bool removeById(const std::string& id);
    
    // For debugging / printing
    Request* getData() const { return data; }
    int getFrontIndex() const { return front; }
    int getCount() const { return count; }
    int getCapacity() const { return capacity; }

private:
    Request* data;
    int capacity;
    int front;
    int rear;
    int count;

    // TODO: Implement to grow capacity (e.g., double the size) when full.
    bool resize(int newCapacity);
    int nextIndex(int idx) const;
};

#endif // REQUEST_QUEUE_H
