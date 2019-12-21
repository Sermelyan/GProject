/*
 * Copyright 2019 <alex>
 */

#include <gtest/gtest.h>
#include "algorithm.hpp"
#include <string>  // only for demonstrate display places names


TEST(test1, algorithm) {

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

}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
