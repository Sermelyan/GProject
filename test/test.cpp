/*
 * Copyright 2019 <alex>
 */

#include <gtest/gtest.h>
#include "worker.hpp"
#include <iostream>
#include "queue.hpp"
#include <thread>


// QUEUE TESTS START
template <class T>
void generateArr(T *arr, size_t arrSize);
template <class T>
void generateRandomArr(T *arr, size_t arrSize);
void threadPush(GQueue<long int> * gq, long int *arr, size_t size );
void threadPop(GQueue<long int> * gq, long int *arr, size_t size );

TEST(create_isEmpty_delete, queue) {
    auto * gq = new GQueue<int>(404);
    auto answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);
    gq->push(777);
    answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 777);
    answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);
    delete gq;
}

TEST(pushMany_popMany, queue) {
    auto * gq = new  GQueue<int>(404);
    for (size_t i = 0 ; i < 1025; i++)
        gq->push(i*i);
    int popValue;
    for (size_t i = 0 ; i < 1025; i++) {
        popValue = gq->popIfNotEmpty();
        ASSERT_TRUE(popValue == i*i);
    }
    auto answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);
    delete gq;
}

TEST(push1_pop1, queue) {
    auto * gq = new  GQueue<int>(404);
    int popValue;
    for (size_t i = 0 ; i < 200; i++) {
        gq->push(i*i);
        popValue = gq->popIfNotEmpty();
        ASSERT_TRUE(popValue == i*i);
        auto answer = gq->popIfNotEmpty();
        ASSERT_TRUE(answer == 404);
    }
    auto answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);
    delete gq;
}

TEST(threads_PushMany_PopMany, queue) {
    auto * gq = new  GQueue<long int>(404);

    auto answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);

    size_t testArrSize = 10000;
    long int * testArrInput = new long int[testArrSize];
    generateArr(testArrInput, testArrSize);

    size_t partSize = testArrSize / 4; // work with 4 threads
    std::thread thrPush1(threadPush, gq, testArrInput, partSize);
    std::thread thrPush2(threadPush, gq, testArrInput + partSize, partSize);
    std::thread thrPush3(threadPush, gq, testArrInput + partSize*2, partSize);
    std::thread thrPush4(threadPush, gq, testArrInput + partSize*3, partSize);

    thrPush1.join();
    thrPush2.join();
    thrPush3.join();
    thrPush4.join();

    long int * testArrOutput = new long int[testArrSize];
    std::thread thrPop1(threadPop, gq, testArrOutput, partSize);
    std::thread thrPop2(threadPop, gq, testArrOutput + partSize, partSize);
    std::thread thrPop3(threadPop, gq, testArrOutput + partSize*2, partSize);
    std::thread thrPop4(threadPop, gq, testArrOutput + partSize*3, partSize);

    thrPop1.join();
    thrPop2.join();
    thrPop3.join();
    thrPop4.join();

    answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);

    std::sort(testArrOutput, testArrOutput + testArrSize);

    for (size_t i = 0 ; i < testArrSize ; i++)  {
        ASSERT_TRUE(testArrInput[i] == testArrOutput[i]);
    }

    delete []testArrInput;
    delete []testArrOutput;
}

TEST(threads_PushMany_PopMany_Random, queue) {
    auto * gq = new  GQueue<long int>(404);

    size_t testArrSize = 20000;

    long int * testArrInput = new long int[testArrSize];
    generateRandomArr(testArrInput, testArrSize);

    long int * testArrOutput = new long int[testArrSize];

    size_t partSize = testArrSize / 4; // work with 4 threads

    std::thread thrPush1(threadPush, gq, testArrInput, partSize);
    std::thread thrPush2(threadPush, gq, testArrInput + partSize, partSize);
    std::thread thrPush3(threadPush, gq, testArrInput + partSize*2, partSize);
    std::thread thrPush4(threadPush, gq, testArrInput + partSize*3, partSize);

    thrPush1.join();
    thrPush2.join();
    thrPush3.join();
    thrPush4.join();

    std::thread thrPop1(threadPop, gq, testArrOutput, partSize);
    std::thread thrPop2(threadPop, gq, testArrOutput + partSize, partSize);
    std::thread thrPop3(threadPop, gq, testArrOutput + partSize*2, partSize);
    std::thread thrPop4(threadPop, gq, testArrOutput + partSize*3, partSize);

    thrPop1.join();
    thrPop2.join();
    thrPop3.join();
    thrPop4.join();

    auto answer = gq->popIfNotEmpty();
    ASSERT_TRUE(answer == 404);
    std::sort(testArrInput, testArrInput + testArrSize);
    std::sort(testArrOutput, testArrOutput + testArrSize);

    for (size_t i = 0 ; i < testArrSize ; i++)  {
        ASSERT_TRUE(testArrInput[i] == testArrOutput[i]);
    }

    delete []testArrInput;
    delete []testArrOutput;

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
        arr[i] = gq->popIfNotEmpty();
    }
}

// QUEUE TESTS END



// WORKER TESTS START
TEST(global, distance) {

    Worker worker;
    const std::string host = "127.0.0.1";
    const std::string target = "/api";
    worker.setHostTarget(host, target);
    std::vector<Point> points;
    points.push_back( Point{3.2,4.5} );
    points.push_back( Point{102.0,-4.7} );
    points.push_back( Point{-7,1.4} );
    points.push_back( Point{72,103.0} );
    points.push_back( Point{-7.3,19.1} );
    points.push_back( Point{-45.4,-182} );

    if (!worker.GetRibsFromAPI(points) ) {
        std::cout<<"\n===API server rip!===\n\n";
        return;
    }


    size_t index;

    // check point "0" -> "1" weight = 99
    index = worker.getWeightIndex(points.size(), 0,1);
    ASSERT_TRUE(worker.edges.at(index).first == 0);
    ASSERT_TRUE(worker.edges.at(index).second == 1);
    ASSERT_TRUE(worker.weightArr.at(index) == 99);

    // check point "0" -> "2" weight = 20
    index = worker.getWeightIndex(points.size(), 0,2);
    ASSERT_TRUE(worker.edges.at(index).first == 0);
    ASSERT_TRUE(worker.edges.at(index).second == 2);
    ASSERT_TRUE(worker.weightArr.at(index) == 10);

    // check point "0" -> "3" weight = 120
    index = worker.getWeightIndex(points.size(), 0,3);
    ASSERT_TRUE(worker.edges.at(index).first == 0);
    ASSERT_TRUE(worker.edges.at(index).second == 3);
    ASSERT_TRUE(worker.weightArr.at(index) == 120);

    // check point "5" -> "0" weight = 192
    index = worker.getWeightIndex(points.size(), 5,0);
    ASSERT_TRUE(worker.edges.at(index).first == 5);
    ASSERT_TRUE(worker.edges.at(index).second == 0);
    ASSERT_TRUE(worker.weightArr.at(index) == 192);

    // check point "5" -> "4" weight = 204
    index = worker.getWeightIndex(points.size(), 5,4);
    ASSERT_TRUE(worker.edges.at(index).first == 5);
    ASSERT_TRUE(worker.edges.at(index).second == 4);
    ASSERT_TRUE(worker.weightArr.at(index) == 204);

    // check point "4" -> "2" weight = 17
    index = worker.getWeightIndex(points.size(), 4,2);
    ASSERT_TRUE(worker.edges.at(index).first == 4);
    ASSERT_TRUE(worker.edges.at(index).second == 2);
    ASSERT_TRUE(worker.weightArr.at(index) == 17);

    // check point "4" -> "2" weight = 17
    index = worker.getWeightIndex(points.size(), 4,2);
    ASSERT_TRUE(worker.edges.at(index).first == 4);
    ASSERT_TRUE(worker.edges.at(index).second == 2);
    ASSERT_TRUE(worker.weightArr.at(index) == 17);

    // check point "2" -> "3" weight = 128
    index = worker.getWeightIndex(points.size(), 2,3);
    ASSERT_TRUE(worker.edges.at(index).first == 2);
    ASSERT_TRUE(worker.edges.at(index).second == 3);
    ASSERT_TRUE(worker.weightArr.at(index) == 128);

    // check point "5" -> "3" weight = 308
    index = worker.getWeightIndex(points.size(), 5,3);
    ASSERT_TRUE(worker.edges.at(index).first == 5);
    ASSERT_TRUE(worker.edges.at(index).second == 3);
    ASSERT_TRUE(worker.weightArr.at(index) == 308);

}


TEST(getIndex, getIndex) {

    size_t index;

    index = Worker::getWeightIndex(10, 3,3);
    ASSERT_TRUE(index == -1);

    index = Worker::getWeightIndex(6, 40,50);
    ASSERT_TRUE(index == -1);

    index = Worker::getWeightIndex(6, 4,5);
    ASSERT_TRUE(index == 24);

    index = Worker::getWeightIndex(6, 1,4);
    ASSERT_TRUE(index == 8);

    index = Worker::getWeightIndex(6, 5,3);
    ASSERT_TRUE(index == 28);

    index = Worker::getWeightIndex(6, 3,0);
    ASSERT_TRUE(index == 15);

    index = Worker::getWeightIndex(6, 0,5);
    ASSERT_TRUE(index == 4);

    index = Worker::getWeightIndex(6, 1,2);
    ASSERT_TRUE(index == 6);

    index = Worker::getWeightIndex(6, 4,1);
    ASSERT_TRUE(index == 21);

    index = Worker::getWeightIndex(6, 2,5);
    ASSERT_TRUE(index == 14);

}


TEST(setJsonAnswerInClass, setJsonAnswerInClass) {

    Worker worker;
    std::string json;
    size_t pointsCount;
    size_t index;

    json = "{\"weights\":{\"0->1\":99,\"0->2\":10,\"0->3\":120,\"0->4\":17,\"0->5\":192,\"1->0\":99,\"1->2\":109,\"1->3\":111,\"1->4\":111,\"1->5\":230,\"2->0\":10,\"2->1\":109,\"2->3\":128,\"2->4\":17,\"2->5\":187,\"3->0\":120,\"3->1\":111,\"3->2\":128,\"3->4\":115,\"3->5\":308,\"4->0\":17,\"4->1\":111,\"4->2\":17,\"4->3\":115,\"4->5\":204,\"5->0\":192,\"5->1\":230,\"5->2\":187,\"5->3\":308,\"5->4\":204}}";
    pointsCount = 6;
    worker.setJsonAnswerInClass(json, pointsCount);

    index = Worker::getWeightIndex(6,0,5);
    ASSERT_TRUE(worker.weightArr.at(index) == 192);
    ASSERT_TRUE(worker.edges.at(index).first == 0);
    ASSERT_TRUE(worker.edges.at(index).second == 5);

    index = Worker::getWeightIndex(6,3,1);
    ASSERT_TRUE(worker.weightArr.at(index) == 111);
    index = Worker::getWeightIndex(6,5,1);
    ASSERT_TRUE(worker.weightArr.at(index) == 230);
    index = Worker::getWeightIndex(6,4,3);
    ASSERT_TRUE(worker.weightArr.at(index) == 115);


    worker.edges.clear();
    worker.weightArr.clear();
    json = "{\"weights\":{\"0->1\":25,\"0->2\":13,\"0->3\":10,\"1->0\":55,\"1->2\":129,\"1->3\":151,\"2->0\":10,\"2->1\":119,\"2->3\":158,\"3->0\":127,\"3->1\":171,\"3->2\":258}}";
    pointsCount = 4;
    worker.setJsonAnswerInClass(json, pointsCount);

    index = Worker::getWeightIndex(4,3,2);
    ASSERT_TRUE(worker.weightArr.at(index) == 258);
    ASSERT_TRUE(worker.edges.at(index).first == 3);
    ASSERT_TRUE(worker.edges.at(index).second == 2);

    index = Worker::getWeightIndex(4,0,1);
    ASSERT_TRUE(worker.weightArr.at(index) == 25);

    index = Worker::getWeightIndex(4,2,0);
    ASSERT_TRUE(worker.weightArr.at(index) == 10);

    index = Worker::getWeightIndex(4,2,3);
    ASSERT_TRUE(worker.weightArr.at(index) == 158);

}


TEST(PythonAPI, PythonAPI) {

    Worker worker;
    std::string jsonForSending = "{\"coordinates\": {\"0\": {\"X\": \"3.2\",\"Y\": \"4.5\"},\"1\": {\"X\": \"102\",\"Y\": \"-4.7\"},\"2\": {\"X\": \"-7\",\"Y\": \"1.39\"}}}";
    std::string answer;

    if ( !worker.getWeightFromPythonAPI(jsonForSending, answer)) {
        std::cout<<"\n===API server rip!===\n\n";
        return;
    }

    std::string goodAnswer = "{\"weights\":{\"0->1\":99,\"0->2\":10,\"1->0\":99,\"1->2\":109,\"2->0\":10,\"2->1\":109}}\n";

    int result = strcmp(answer.c_str(), goodAnswer.c_str());
    ASSERT_TRUE(result == 0);

    jsonForSending = "{\"coordinates\": {\"0\": {\"X\": \"0\",\"Y\": \"10.0\"},\"1\": {\"X\": \"2\",\"Y\": \"0\"},\"2\": {\"X\": \"-1\",\"Y\": \"-1\"}}}";

    worker.getWeightFromPythonAPI(jsonForSending, answer);

    goodAnswer = "{\"weights\":{\"0->1\":10,\"0->2\":11,\"1->0\":10,\"1->2\":3,\"2->0\":11,\"2->1\":3}}\n";

    result = strcmp(answer.c_str(), goodAnswer.c_str());
    ASSERT_TRUE(result == 0);

}


TEST(createJsonForSend, createJsonForSend) {

    Worker worker;
    std::vector<Point> points;
    points.push_back( Point{3.2,4.5} );
    points.push_back( Point{102.0,-4.7} );
    points.push_back( Point{-7,1.4} );
    points.push_back( Point{72,103.0} );

    std::string answer = worker.createJsonForSending(points);

    std::string goodAnswer = "{\n"
                             "    \"coordinates\": {\n"
                             "        \"0\": {\n"
                             "            \"X\": \"3.2000000000000002\",\n"
                             "            \"Y\": \"4.5\"\n"
                             "        },\n"
                             "        \"1\": {\n"
                             "            \"X\": \"102\",\n"
                             "            \"Y\": \"-4.7000000000000002\"\n"
                             "        },\n"
                             "        \"2\": {\n"
                             "            \"X\": \"-7\",\n"
                             "            \"Y\": \"1.3999999999999999\"\n"
                             "        },\n"
                             "        \"3\": {\n"
                             "            \"X\": \"72\",\n"
                             "            \"Y\": \"103\"\n"
                             "        }\n"
                             "    }\n"
                             "}\n";

    int result = strcmp(answer.c_str(), goodAnswer.c_str());
    ASSERT_TRUE(result == 0);

}

// WORKER TESTS END



// ALGO TEST START



// ALGO TEST END


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
