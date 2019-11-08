/*
 * Copyright 2019 <Copyright Owner>
 */

#ifndef INCLUDE_QUEU_HPP_
#define INCLUDE_QUEU_HPP_

#include <iostream>  // delete line and std::cout
#include <vector>
#include <mutex>


template <class T>
class GQueue{

public:
    GQueue();
    GQueue(const size_t &startSize);
    ~GQueue();
    void push(const  T &element);
    T pop();
    bool isEmpty();

    GQueue(const GQueue&) = delete;
    GQueue(const GQueue&&) = delete;
    GQueue& operator=(const GQueue&) = delete;
    GQueue& operator=(const GQueue&&) = delete;

private:
    size_t BUFFER_START_SIZE = 8;
    unsigned char RESIZE_CONST = 2;

    std::vector<T> * arr = nullptr;
    size_t buffersize  = 0;
    size_t head = 0;
    size_t tail = 0;
    std::mutex GQueueMutex;

    void resize();
};

// END OF HEADER FILE!
// below realization ONLY for tests




template <class T>
GQueue<T>::GQueue(){
    arr = new std::vector<T>;
    arr->resize(BUFFER_START_SIZE);
    buffersize = BUFFER_START_SIZE;
    head = 0;
    tail = 0;
    //std::cout<<"constructor STANDART START SIZE!\n";
}


template <class T>
GQueue<T>::GQueue(const size_t &startSize){
    arr = new std::vector<T>;
    arr->resize(startSize);
    buffersize = startSize;
    head = 0;
    tail = 0;
    //std::cout<<"constructor!\n";
}


template <class T>
GQueue<T>::~GQueue(){
    delete arr;
    //std::cout<<"destructor!\n";
}


template <class T>
void GQueue<T>::push( const T &element){

    GQueueMutex.lock();

    //std::cout<<"size:"<<head + 1<<" == "<< buffersize <<"\n";
    if (head + 1 == buffersize) {
        arr->resize(arr->size() * RESIZE_CONST);
        buffersize = arr->size();
    }

    arr->at(head) = element;
    head++;

    GQueueMutex.unlock();
}


template <class T>
T GQueue<T>::pop(){
    GQueueMutex.lock();

    T &answer = arr->at(tail);
    tail++;
    //std::cout<<"answer:"<<answer<<"\n";

    GQueueMutex.unlock();
    return answer;
}


template <class T>
bool GQueue<T>::isEmpty(){
    GQueueMutex.lock();

    bool answer = head == tail;

    GQueueMutex.unlock();
    return answer;
}


template <class T>
void GQueue<T>::resize(){
    //pass
}


// delete class
class Foo{
public:
    void fooFun();
};


#endif  //  INCLUDE_QUEU_HPP_
