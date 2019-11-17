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
    explicit Algorithm(std::vector<edge> edgeArr, std::vector<weight> weightArr);
    ~Algorithm();
    Algorithm(const Algorithm&) = delete;
    Algorithm(const Algorithm&&) = delete;
    Algorithm& operator=(const Algorithm&) = delete;
    Algorithm& operator=(const Algorithm&&) = delete;

    std::vector<dotId> CalcRoute(dotId A, dotId B);

 private:
    void MakeGraph();
    edge * edgeArr;
    size_t edgeSize;
    weight * weightArr;
    graph_t myGraph;
};
#endif  //  INCLUDE_ALGORITHM_HPP_
