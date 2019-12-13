/*
 * Copyright 2019 <Alex>
 */


#include <boost/graph/adjacency_list.hpp>


#ifndef INCLUDE_ALGORITHM_HPP_
#define INCLUDE_ALGORITHM_HPP_


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
    getRoute(dotId from, const size_t &pointsCount,
            const size_t &time, const size_t &maxPlacesCount);



    // delete this from algo and use from WorkerAPI
    long int getWeightIndex(const size_t &pointsCount, const size_t &from, const size_t &to);

 private:
    void MakeGraph();
    std::unique_ptr<edge> edgeArr;
    std::unique_ptr<weight> weightArr;
    size_t edgeSize;

    graph_t myGraph;
    const size_t MAX_PLACES = 200;  // контроль (максимальное кол-во мест в пути)
};
#endif  //  INCLUDE_ALGORITHM_HPP_
