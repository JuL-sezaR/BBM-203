#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    (void)req; // suppress unused warning until implemented

    if (isFull()) {
        resize(capacity * 2);
    }

    data[rear] = req;
    rear = nextIndex(rear);
    count++;
    return true;
}

void RequestQueue::enqueueFront(const Request& req) {
    if (isFull()) {
        resize(capacity * 2);
    }

    front = (front - 1 + capacity) % capacity;
    data[front] = req;
    count++;
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    (void)outReq; // suppress unused warning until implemented

    if (isEmpty()) return false;
    else {
        outReq = data[front];
        front = nextIndex(front);
        count--;
        return true;
    }
    return false;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.

    if (isEmpty()) return false;
    else {
        outReq = data[front];
    }
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    (void)id; // suppress unused warning until implemented
    
    bool found = false;
    Request* tempData = new Request[capacity];
    int tempcount = 0;


    for (int i = 0; i < count; i++) {
        int idx = (front + i) % capacity;

        if (data[idx].getId() != id) {
            tempData[tempcount++] = data[idx];
        } else {
            found = true;
        }
    }

    if (found) {
        delete[] data;
        data = tempData;
        front = 0;
        rear = tempcount;
        count = tempcount;
        return true;
    }
    delete[] tempData;
    return false;
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.

    if (newCapacity <= count) {
        return true;
    }

    Request* newData = new (std::nothrow) Request[newCapacity];
    if (!newData) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        newData[i] = data[(front + i) % capacity];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
    front = 0;
    rear = count;
    return true;
}
bool RequestQueue::getAt(int index, Request& out) const {
    if (index < 0 || index >= count) return false;

    int realIndex = (front + index) % capacity;
    out = data[realIndex];
    return true;
}

