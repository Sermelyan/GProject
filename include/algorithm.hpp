/*
 * Copyright 2019 <Alex>
 */

#ifndef INCLUDE_ALGORITHM_HPP_
#define INCLUDE_ALGORITHM_HPP_
#include <utility>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <assert.h>

#include <iostream>  // delete after tests

class Algorithm {

 public:
    typedef std::size_t dotId;
    typedef std::pair<dotId, dotId> edge;
    typedef std::size_t weight;

 private:
    typedef boost::adjacency_list <boost::listS, boost::vecS, boost::directedS,
    boost::no_property,boost::property<boost::edge_weight_t, size_t> > graph_t;
    typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;

 public:
    Algorithm()= delete;
    explicit Algorithm(const std::vector<edge> &edgeArr, const std::vector<weight> &weightArr);
    ~Algorithm();
    Algorithm(const Algorithm&) = delete;
    Algorithm(const Algorithm&&) = delete;
    Algorithm& operator=(const Algorithm&) = delete;
    Algorithm& operator=(const Algorithm&&) = delete;

    std::vector<dotId> CalcRoute(const dotId &A, const dotId &B);

    std::pair<std::vector<Algorithm::dotId>, size_t>
    getRoute(const dotId &from, const size_t &pointsCount, const size_t &time);

    void recBFS(const Algorithm::dotId &from, const size_t &pointsCount,
                const size_t &time, size_t &currentTime, std::vector<dotId> &visitedPints,
                std::pair<std::vector<Algorithm::dotId>, size_t> &answer);

    // delete this from algo and use from WorkerAPI
    long int getWeightIndex(const size_t &pointsCount, const size_t &from, const size_t &to);

 private:
    void MakeGraph();
    edge * edgeArr;
    size_t edgeSize;
    weight * weightArr;
    graph_t myGraph;
    const size_t MAX_RECURSION = 200; // контроль рекурсии (максимальное кол-во мест в пути)
};
#endif  //  INCLUDE_ALGORITHM_HPP_
