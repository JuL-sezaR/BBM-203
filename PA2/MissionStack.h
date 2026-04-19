#ifndef MISSION_STACK_H
#define MISSION_STACK_H

#include "Request.h"

class MissionStack {
public:
    MissionStack();
    explicit MissionStack(int initialCapacity);
    ~MissionStack();

    MissionStack(const MissionStack&) = delete;
    MissionStack& operator=(const MissionStack&) = delete;

    bool isEmpty() const;
    int size() const;

    // Push a new request onto the stack.
    // If the underlying array is full, resize it (e.g., double capacity).
    // TODO: Implement in MissionStack.cpp.
    bool push(const Request& req);

    // Pop the top request into 'outReq'.
    // Returns false if stack is empty.
    // TODO: Implement in MissionStack.cpp.
    bool pop(Request& outReq);

    // Peek at the top request without removing.
    bool peek(Request& outReq) const;

    // Remove all elements from the stack (but keep allocated array).
    void clear();

    Request* getData() const { return data; }
    int getTopIndex() const { return top; }
    int getCapacity() const { return capacity; }

private:
    Request* data;
    int capacity;
    int top; // index of last valid element, -1 when empty

    bool resize(int newCapacity);
};

#endif // MISSION_STACK_H
