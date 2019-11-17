/*
 * Copyright 2019 <Copyright Owner>
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


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
