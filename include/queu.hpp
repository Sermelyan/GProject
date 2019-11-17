/*
 * Copyright 2019 <Alex>
 */

#ifndef INCLUDE_QUEU_HPP_
#define INCLUDE_QUEU_HPP_

#include <queue>
#include <mutex>

template <class T>
class GQueue {
 public:
    GQueue()=delete;
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
    std::queue<T> * queue;
    std::mutex GQueueMutex;
};


template <class T>
GQueue<T>::~GQueue() {
    delete queue;
}

template <class T>
GQueue<T>::GQueue(const T &element404){
    queue = new std::queue<T>;
    this->element404 =  element404;
}

template <class T>
void GQueue<T>::push(const T &element) {
    GQueueMutex.lock();
    queue->push(element);
    GQueueMutex.unlock();
}

template <class T>
T GQueue<T>::popIfNotEmpty() {
    GQueueMutex.lock();
    T answer = this->element404;
    if (!queue->empty()) {
        answer = queue->front();
        queue->pop();
    }
    GQueueMutex.unlock();
    return answer;
}

#endif  //  INCLUDE_QUEU_HPP_
