/*
 * Copyright 2019 <Alex>
 */

#ifndef INCLUDE_QUEUE_HPP_
#define INCLUDE_QUEUE_HPP_

#include <queue>
#include <mutex>
#include <memory>

template <class T>
class GQueue {
public:
    GQueue() = delete;
    explicit GQueue(const T &element404);
    ~GQueue();
    void push(const  T &element);
    T  popIfNotEmpty();

    GQueue(const GQueue&) = delete;
    GQueue(const GQueue&&) = delete;
    GQueue& operator=(const GQueue&) = delete;
    GQueue& operator=(const GQueue&&) = delete;

private:
    T element404;
    std::unique_ptr<std::queue<T>> queue;
    std::mutex GQueueMutex;
};


template <class T>
GQueue<T>::~GQueue() {
}

template <class T>
GQueue<T>::GQueue(const T &element404) {
    queue.reset(new std::queue<T>);
    this->element404 =  element404;
}

template <class T>
void GQueue<T>::push(const T &element) {
    const std::lock_guard<std::mutex> lock(GQueueMutex);
    queue->push(element);
}

template <class T>
T GQueue<T>::popIfNotEmpty() {
    const std::lock_guard<std::mutex> lock(GQueueMutex);
    T answer = this->element404;
    if (!queue->empty()) {
        answer = queue->front();
        queue->pop();
    }
    return answer;
}

#endif  //  INCLUDE_QUEU_HPP_