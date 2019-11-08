/*
 * Copyright 2019 <Copyright Owner>
 */

#include <gtest/gtest.h>
#include <iostream>
#include "queu.hpp"
#include <thread>

template <class T>
void generateArr(T *arr, size_t arrSize);
template <class T>
void generateRandomArr(T *arr, size_t arrSize);
void threadPush(GQueue<long int> * gq, long int *arr, size_t size );
void threadPop(GQueue<long int> * gq, long int *arr, size_t size );




TEST(create_isEmpty_delete, queue) {
    auto * gq = new  GQueue<int>(256);
    ASSERT_TRUE(gq->isEmpty());
    gq->push(777);
    ASSERT_FALSE(gq->isEmpty());
    gq->pop();
    ASSERT_TRUE(gq->isEmpty());
    delete gq;
}

TEST(pushMany_popMany, queue) {
    auto * gq = new  GQueue<int>(16);
    for (size_t i = 0 ; i < 1025; i++)
        gq->push(i*i);
    int popValue;
    for (size_t i = 0 ; i < 1025; i++) {
        popValue = gq->pop();
        //std::cout<<"test pushMany_popMany: "<< popValue<< " == "<<   i*i<<"\n";
        ASSERT_TRUE(popValue == i*i);
    }
    ASSERT_TRUE(gq->isEmpty());
    delete gq;
}

TEST(push1_pop1, queue) {
    auto * gq = new  GQueue<int>(8);
    int popValue;
    for (size_t i = 0 ; i < 200; i++) {
        gq->push(i*i);
        popValue = gq->pop();
        //std::cout<<"test push1_pop1: "<< popValue<< " == "<<   i*i<<"\n";
        ASSERT_TRUE(popValue == i*i);
    }
    ASSERT_TRUE(gq->isEmpty());
    delete gq;
}

TEST(threads_PushMany_PopMany, queue) {
    auto * gq = new  GQueue<long int>(32);
    ASSERT_TRUE(gq->isEmpty());

    size_t testArrSize = 10000;
    long int * testArrInput = new long int[testArrSize];
    generateArr(testArrInput, testArrSize);

    size_t partSize = testArrSize / 4; // work with 4 threads
    std::thread thrPush1(threadPush, gq, testArrInput, partSize);   //void threadPush(GQueue<T> * gq, T *arr, size_t size )
    std::thread thrPush2(threadPush, gq, testArrInput + partSize, partSize);
    std::thread thrPush3(threadPush, gq, testArrInput + partSize*2, partSize);
    std::thread thrPush4(threadPush, gq, testArrInput + partSize*3, partSize);

    thrPush1.join();
    thrPush2.join();
    thrPush3.join();
    thrPush4.join();
    ASSERT_FALSE(gq->isEmpty());

    long int * testArrOutput = new long int[testArrSize];
    std::thread thrPop1(threadPop, gq, testArrOutput, partSize);   //void threadPush(GQueue<T> * gq, T *arr, size_t size )
    std::thread thrPop2(threadPop, gq, testArrOutput + partSize, partSize);
    std::thread thrPop3(threadPop, gq, testArrOutput + partSize*2, partSize);
    std::thread thrPop4(threadPop, gq, testArrOutput + partSize*3, partSize);

    thrPop1.join();
    thrPop2.join();
    thrPop3.join();
    thrPop4.join();

    std::sort(testArrOutput, testArrOutput + testArrSize);

    for (size_t i = 0 ; i < testArrSize ; i++)  {
        //std::cout<<testArrInput[i] <<" == "<< testArrOutput[i] <<"\n";
        ASSERT_TRUE(testArrInput[i] == testArrOutput[i]);
    }

    ASSERT_TRUE(gq->isEmpty());

    delete []testArrInput;
    delete []testArrOutput;
    delete gq;
}

TEST(threads_PushMany_PopMany_Random, queue) {
    auto * gq = new  GQueue<long int>(32);
    ASSERT_TRUE(gq->isEmpty());

    //testArrSize >= thread count and testArrSize % thread(4) == 0 (условие для отдачи потокам - не функционал очереди)
    size_t testArrSize = 20000;

    long int * testArrInput = new long int[testArrSize];
    generateRandomArr(testArrInput, testArrSize);

    long int * testArrOutput = new long int[testArrSize];

    size_t partSize = testArrSize / 4; // work with 4 threads

    std::thread thrPush1(threadPush, gq, testArrInput, partSize);   //void threadPush(GQueue<T> * gq, T *arr, size_t size )
    std::thread thrPush2(threadPush, gq, testArrInput + partSize, partSize);
    std::thread thrPush3(threadPush, gq, testArrInput + partSize*2, partSize);
    std::thread thrPush4(threadPush, gq, testArrInput + partSize*3, partSize);

    thrPush1.join();
    thrPush2.join();
    thrPush3.join();
    thrPush4.join();
    ASSERT_FALSE(gq->isEmpty());

    std::thread thrPop1(threadPop, gq, testArrOutput, partSize);   //void threadPush(GQueue<T> * gq, T *arr, size_t size )
    std::thread thrPop2(threadPop, gq, testArrOutput + partSize, partSize);
    std::thread thrPop3(threadPop, gq, testArrOutput + partSize*2, partSize);
    std::thread thrPop4(threadPop, gq, testArrOutput + partSize*3, partSize);

    thrPop1.join();
    thrPop2.join();
    thrPop3.join();
    thrPop4.join();

    std::sort(testArrInput, testArrInput + testArrSize);
    std::sort(testArrOutput, testArrOutput + testArrSize);

    for (size_t i = 0 ; i < testArrSize ; i++)  {
        //std::cout<<testArrInput[i] <<" == "<< testArrOutput[i] <<"\n";
        ASSERT_TRUE(testArrInput[i] == testArrOutput[i]);
    }

    ASSERT_TRUE(gq->isEmpty());

    delete []testArrInput;
    delete []testArrOutput;
    delete gq;
}





int main(int argc, char** argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}


template <class T>
void generateArr(T *arr, size_t arrSize){
    for (size_t i = 0 ; i <arrSize; i++)
        arr[i] = i;
}

template <class T>
void generateRandomArr(T *arr, size_t arrSize) {
    size_t  KOEF = 1; // if need more RAND_MAX
    size_t MAX_VALUE = 100;
    for (size_t i = 0 ; i <arrSize; i++)
        arr[i] = (rand() * KOEF) % MAX_VALUE;
}

void threadPush(GQueue<long int> * gq, long int *arr, size_t size ) {
    for (size_t i = 0; i < size; i++ ){
        gq->push(arr[i]);
    }
}

void threadPop(GQueue<long int> * gq, long int *arr, size_t size ) {
    for (size_t i = 0; i < size; i++ ){
        arr[i] = gq->pop();
    }
}