/*
 * Copyright 2019 <Alex>
 */

#include "algorithm.hpp"

Algorithm::Algorithm(std::vector<edge> edges, std::vector<weight> weights) {

    assert(edges.size() == weights.size());

    edgeSize = edges.size();
    edgeArr = new edge[edgeSize];
    weightArr = new weight[edgeSize];
    for (size_t i = 0; i < edgeSize; i++) {
        *(edgeArr+i) = edges.at(i);
        *(weightArr+i)= weights.at(i);
    }
}

Algorithm::~Algorithm() {
    delete[]edgeArr;
    delete[]weightArr;
}

void Algorithm::MakeGraph() {
    myGraph = graph_t(edgeArr, edgeArr + edgeSize, weightArr, edgeSize);
    boost::property_map<graph_t, boost::edge_weight_t>::type weightGraph = get(boost::edge_weight, myGraph);
}

std::vector<Algorithm::dotId> Algorithm::CalcRoute(dotId A, dotId B) {

    MakeGraph();

    // вектор для пройденных путей
    std::vector<vertex_descriptor> pastWay(num_vertices(myGraph));
    // вектор для расстояния от корня
    std::vector<size_t> rootDist(num_vertices(myGraph));

    // Обозначим начальную и конечную точку
    vertex_descriptor pointA = vertex(A, myGraph);
    vertex_descriptor pointB = vertex(B, myGraph);

    boost::dijkstra_shortest_paths(myGraph, pointA,
                                   boost::predecessor_map(&pastWay[0]).distance_map(&rootDist[0]));

    std::vector<boost::graph_traits<graph_t>::vertex_descriptor> path;
    boost::graph_traits<graph_t>::vertex_descriptor current = pointB;

    // идем в обратную сторону и запоминаем путь
    while(current != pointA) {
        path.push_back(current);
        current = pastWay[current];
    }
    path.push_back(pointA); // добавляем в конец точку с которой начинали

    std::vector<dotId> answer;

    // в ответ записываем в правильном порядке
    std::vector<boost::graph_traits<graph_t>::vertex_descriptor >::reverse_iterator iter;
    for (iter = path.rbegin(); iter != path.rend(); ++iter)
        answer.push_back(*iter);

    return answer;
}

