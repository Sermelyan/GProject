/*
 * Copyright 2019 <alex>
 */

#include <gtest/gtest.h>
#include "worker.hpp"
#include <iostream>
#include "queue.hpp"
#include <thread>
#include "algorithm.hpp"
#include <string>  // only for demonstrate display places names

int allTest(){
    return RUN_ALL_TESTS();
}
#define DEBUG 1



int main(int argc, char** argv) {


    std::cout<<"\n  START MAIN!  \n";
    if (DEBUG) {
        testing::InitGoogleTest(&argc, argv);
        allTest();

    }

    // тут будет запуск всего и вся...



    std::cout<<"\n  END MAIN!  \n";

    return 0;
}











// QUEUE TESTS START

template <class T>
void generateArr(T *arr, size_t arrSize);
template <class T>
void generateRandomArr(T *arr, size_t arrSize);
void threadPush(GQueue<long int> * gq, long int *arr, size_t size );
void threadPop(GQueue<long int> * gq, long int *arr, size_t size );

TEST(create_isEmpty_delete, queue) {
    std::cout<<"\n============= START QUEUE TESTS! ================\n";
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
    std::cout<<"============= END QUEUE TESTS! ================\n\n\n";

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
    std::cout<<"\n============= START WORKER TESTS! ================\n";
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
    std::cout<<"============= END WORKER TESTS! ================\n\n\n";

}

// WORKER TESTS END



// ALGO TEST START

TEST(test1, algorithm) {
    std::cout<<"\n============= START ALGO TESTS! ================\n";
    //  у нас есть места, для алгоритма место - просто его id(size_t)
    //  dotId A=0, B=1, C=2, D=3, E=4, F=5;
    enum nodes { A, B, C, D, E, F }; // id мест
    // строковое название места - для отображения id -> название места
    // id соответствует индексу в массиве
    std::string names[] = {"point-A", "point-B", "point-C",
                           "point-D", "point-E", "point-F"};

    std::vector<Algorithm::edge> edges; // создадим пути
    std::vector<Algorithm::weight> weights; // создадим метрики для путей
    edges.push_back(std::make_pair(C,A));  weights.push_back(10);
    edges.push_back(std::make_pair(A,B));  weights.push_back(7);
    edges.push_back(std::make_pair(A,E));  weights.push_back(8);
    edges.push_back(std::make_pair(F,A));  weights.push_back(6);
    edges.push_back(std::make_pair(B,C));  weights.push_back(8);
    edges.push_back(std::make_pair(B,D));  weights.push_back(12);
    edges.push_back(std::make_pair(E,C));  weights.push_back(10);
    edges.push_back(std::make_pair(E,F));  weights.push_back(8);
    edges.push_back(std::make_pair(D,F));  weights.push_back(1);
    edges.push_back(std::make_pair(D,E));  weights.push_back(13);
    edges.push_back(std::make_pair(F,B));  weights.push_back(5);

    // кол-во метрик должно быть равно количеству путей!
    ASSERT_TRUE(edges.size() == weights.size());

    Algorithm alg = Algorithm(edges, weights);

    // найдем путь A -> F
    std::vector<Algorithm::dotId> answer = alg.CalcRoute(A, F);

    //строковой вывод названий мест
    std::cout<<"Answer: ";
    for (size_t i =0; i < answer.size(); i++)
        std::cout<<names[answer.at(i)]<<" ";
    std::cout<<"\nAnswer need: point-A point-E point-F\n";

    // A E F
    ASSERT_TRUE(answer.at(0) == A && answer.at(1) == E && answer.at(2) == F);
}

TEST(test2, algorithm) {


    enum nodes { A, B, C, D, E, F };

    std::vector<Algorithm::edge> edges; // создадим пути
    std::vector<Algorithm::weight> weights; // создадим метрики для путей
    edges.push_back(std::make_pair(C,A));  weights.push_back(10);
    edges.push_back(std::make_pair(A,B));  weights.push_back(7);
    edges.push_back(std::make_pair(A,E));  weights.push_back(8);
    edges.push_back(std::make_pair(F,A));  weights.push_back(6);
    edges.push_back(std::make_pair(B,C));  weights.push_back(8);
    edges.push_back(std::make_pair(B,D));  weights.push_back(12);
    edges.push_back(std::make_pair(E,C));  weights.push_back(10);
    edges.push_back(std::make_pair(E,F));  weights.push_back(8);
    edges.push_back(std::make_pair(D,F));  weights.push_back(1);
    edges.push_back(std::make_pair(D,E));  weights.push_back(13);
    edges.push_back(std::make_pair(F,B));  weights.push_back(5);


    ASSERT_TRUE(edges.size() == weights.size());

    Algorithm alg = Algorithm(edges, weights);

    std::vector<Algorithm::dotId> answer = alg.CalcRoute(F, C);
    // F B C
    ASSERT_TRUE(answer.at(0) == F && answer.at(1) == B && answer.at(2) == C);
}

TEST(test3, algorithm) {


    enum nodes { A, B, C, D, E, F };

    std::vector<Algorithm::edge> edges; // создадим пути
    std::vector<Algorithm::weight> weights; // создадим метрики для путей
    edges.push_back(std::make_pair(C,A));  weights.push_back(2);
    edges.push_back(std::make_pair(A,B));  weights.push_back(1);
    edges.push_back(std::make_pair(A,E));  weights.push_back(8);
    edges.push_back(std::make_pair(F,A));  weights.push_back(6);
    edges.push_back(std::make_pair(B,C));  weights.push_back(8);
    edges.push_back(std::make_pair(B,D));  weights.push_back(3);
    edges.push_back(std::make_pair(E,C));  weights.push_back(10);
    edges.push_back(std::make_pair(E,F));  weights.push_back(8);
    edges.push_back(std::make_pair(D,F));  weights.push_back(2);
    edges.push_back(std::make_pair(D,E));  weights.push_back(13);
    edges.push_back(std::make_pair(F,B));  weights.push_back(5);


    ASSERT_TRUE(edges.size() == weights.size());

    Algorithm alg = Algorithm(edges, weights);

    std::vector<Algorithm::dotId> answer = alg.CalcRoute(C, F);
    // C A B D F
    ASSERT_TRUE(answer.at(0) == C && answer.at(1) == A && answer.at(2) == B
                && answer.at(3) == D && answer.at(4) == F);
}

TEST(test4_2ways, algorithm) {

    enum nodes { A, B, C, D, E, F };

    std::vector<Algorithm::edge> edges; // создадим пути
    std::vector<Algorithm::weight> weights; // создадим метрики для путей
    edges.push_back(std::make_pair(C,A));  weights.push_back(10);
    edges.push_back(std::make_pair(A,B));  weights.push_back(7);
    edges.push_back(std::make_pair(A,E));  weights.push_back(8);
    edges.push_back(std::make_pair(F,A));  weights.push_back(6);
    edges.push_back(std::make_pair(B,C));  weights.push_back(8);
    edges.push_back(std::make_pair(B,D));  weights.push_back(12);
    edges.push_back(std::make_pair(E,C));  weights.push_back(10);
    edges.push_back(std::make_pair(E,F));  weights.push_back(8);
    edges.push_back(std::make_pair(D,F));  weights.push_back(1);
    edges.push_back(std::make_pair(D,E));  weights.push_back(13);
    edges.push_back(std::make_pair(F,B));  weights.push_back(5);

    // we already have E C way
    edges.push_back(std::make_pair(C,E));  weights.push_back(4);


    ASSERT_TRUE(edges.size() == weights.size());

    Algorithm alg = Algorithm(edges, weights);

    std::vector<Algorithm::dotId> answer = alg.CalcRoute(B, E);
    // B C E
    ASSERT_TRUE(answer.at(0) == B && answer.at(1) == C && answer.at(2) == E);
}

TEST(test5_manyUse, algorithm) {

    enum nodes { A, B, C, D, E, F };

    std::vector<Algorithm::edge> edges; // создадим пути
    std::vector<Algorithm::weight> weights; // создадим метрики для путей
    edges.push_back(std::make_pair(C,A));  weights.push_back(10);
    edges.push_back(std::make_pair(A,B));  weights.push_back(7);
    edges.push_back(std::make_pair(A,E));  weights.push_back(8);
    edges.push_back(std::make_pair(F,A));  weights.push_back(9);
    edges.push_back(std::make_pair(B,C));  weights.push_back(8);
    edges.push_back(std::make_pair(B,D));  weights.push_back(12);
    edges.push_back(std::make_pair(E,C));  weights.push_back(10);
    edges.push_back(std::make_pair(E,F));  weights.push_back(8);
    edges.push_back(std::make_pair(D,F));  weights.push_back(1);
    edges.push_back(std::make_pair(D,E));  weights.push_back(13);
    edges.push_back(std::make_pair(F,B));  weights.push_back(5);

    edges.push_back(std::make_pair(D,B));  weights.push_back(2);
    edges.push_back(std::make_pair(B,A));  weights.push_back(5);

    ASSERT_TRUE(edges.size() == weights.size());

    Algorithm alg = Algorithm(edges, weights);

    std::vector<Algorithm::dotId> answer = alg.CalcRoute(D, A);
    // D B A
    ASSERT_TRUE(answer.at(0) == D && answer.at(1) == B && answer.at(2) == A);

    answer = alg.CalcRoute(F, C);
    // F B C
    ASSERT_TRUE(answer.at(0) == F && answer.at(1) == B && answer.at(2) == C);

    answer = alg.CalcRoute(E, C);
    // E C
    ASSERT_TRUE(answer.at(0) == E && answer.at(1) == C);

    answer = alg.CalcRoute(B, B); // senseless
    // B
    ASSERT_TRUE(answer.at(0) == B);
}


// tests for getRoute for interests
TEST(test6getRoute, algorithm) {

    std::vector<Algorithm::edge> edges;
    std::vector<Algorithm::weight > weights;

    // from 0
    edges.push_back(std::make_pair(0,1));
    weights.push_back(7);
    edges.push_back(std::make_pair(0,2));
    weights.push_back(4);
    edges.push_back(std::make_pair(0,3));
    weights.push_back(18);
    edges.push_back(std::make_pair(0,4));
    weights.push_back(10);

    // from 1
    edges.push_back(std::make_pair(1,0));
    weights.push_back(7);
    edges.push_back(std::make_pair(1,2));
    weights.push_back(9);
    edges.push_back(std::make_pair(1,3));
    weights.push_back(10);
    edges.push_back(std::make_pair(1,4));
    weights.push_back(31);

    // from 2
    edges.push_back(std::make_pair(2,0));
    weights.push_back(4);
    edges.push_back(std::make_pair(2,1));
    weights.push_back(9);
    edges.push_back(std::make_pair(2,3));
    weights.push_back(21);
    edges.push_back(std::make_pair(2,4));
    weights.push_back(15);

    // from 3
    edges.push_back(std::make_pair(3,0));
    weights.push_back(18);
    edges.push_back(std::make_pair(3,1));
    weights.push_back(10);
    edges.push_back(std::make_pair(3,2));
    weights.push_back(21);
    edges.push_back(std::make_pair(3,4));
    weights.push_back(5);

    // from 4
    edges.push_back(std::make_pair(4,0));
    weights.push_back(10);
    edges.push_back(std::make_pair(4,1));
    weights.push_back(31);
    edges.push_back(std::make_pair(4,2));
    weights.push_back(15);
    edges.push_back(std::make_pair(4,3));
    weights.push_back(5);

    Algorithm algo = Algorithm(edges, weights);

    std::pair<std::vector<Algorithm::dotId>, size_t> answer;

    size_t time = 24;
    answer = algo.getRoute(2, 5, time, 4);
    ASSERT_TRUE(answer.first.size() == 4 && answer.second == 21);

    // не времени никуда добраться
    time = 3;
    answer = algo.getRoute(2, 5, time, 3);
    ASSERT_TRUE(answer.first.size() == 1 && answer.second == 0);

    // граничное условие на время
    time = 11;
    answer = algo.getRoute(2, 5, time, 3);
    ASSERT_TRUE(answer.first.size() == 3 && answer.second == 11);

    //  много времени
    time = 1100;
    answer = algo.getRoute(4, 5, time, 5);
    ASSERT_TRUE(answer.first.size() == 5 && answer.second == 26);
    ASSERT_TRUE(answer.first.at(0)== 4 && answer.first.at(1)== 3 &&
                answer.first.at(2)== 1 && answer.first.at(3)== 0 &&
                answer.first.at(4)== 2);

    //  много времени но не более 2 точек
    time = 1100;
    answer = algo.getRoute(1, 5, time, 2);
    ASSERT_TRUE(answer.first.size() == 3 && answer.second == 11);
    ASSERT_TRUE(answer.first.at(0)== 1 && answer.first.at(1)== 0 &&
                answer.first.at(2)== 2);

    time = 1100;
    answer = algo.getRoute(1, 5, time, 2);
    ASSERT_TRUE(answer.first.size() == 3 && answer.second == 11);
    ASSERT_TRUE(answer.first.at(0)== 1 && answer.first.at(1)== 0 &&
                answer.first.at(2)== 2);

    time = 17;
    answer = algo.getRoute(3, 5, time, 3);
    ASSERT_TRUE(answer.first.size() == 3 && answer.second == 15);
    ASSERT_TRUE(answer.first.at(0)== 3 && answer.first.at(1)== 4 &&
                answer.first.at(2)== 0);

    time = 19;
    answer = algo.getRoute(3, 5, time, 3);
    ASSERT_TRUE(answer.first.size() == 4 && answer.second == 19);
    ASSERT_TRUE(answer.first.at(0)== 3 && answer.first.at(1)== 4 &&
                answer.first.at(3)== 2);

    time = 100;
    answer = algo.getRoute(0, 5, time, 3);
    ASSERT_TRUE(answer.first.size() == 4 && answer.second == 23);
    ASSERT_TRUE(answer.first.at(0)== 0 && answer.first.at(1)== 2 &&
                answer.first.at(3)== 3);

    time = 100;
    answer = algo.getRoute(0, 5, time, 4);
    ASSERT_TRUE(answer.first.size() == 5 && answer.second == 28);
    ASSERT_TRUE(answer.first.at(0)== 0 && answer.first.at(1)== 2 &&
                answer.first.at(4)== 4);
    std::cout<<"============= END ALGO TESTS! ================\n\n\n";
}

// ALGO TEST END