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
    edges.push_back(std::make_pair(C,A));
    edges.push_back(std::make_pair(A,B));
    edges.push_back(std::make_pair(A,E));
    edges.push_back(std::make_pair(F,A));
    edges.push_back(std::make_pair(B,C));
    edges.push_back(std::make_pair(B,D));
    edges.push_back(std::make_pair(E,C));
    edges.push_back(std::make_pair(E,F));
    edges.push_back(std::make_pair(D,F));
    edges.push_back(std::make_pair(D,E));
    edges.push_back(std::make_pair(F,B));

    std::vector<Algorithm::weight> weights; // создадим метрики для путей
    weights.push_back(10);
    weights.push_back(7);
    weights.push_back(8);
    weights.push_back(6);
    weights.push_back(8);
    weights.push_back(12);
    weights.push_back(10);
    weights.push_back(8);
    weights.push_back(1);
    weights.push_back(13);
    weights.push_back(5);

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



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
