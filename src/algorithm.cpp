/*
 * Copyright 2019 <Alex>
 */

#include "algorithm.hpp"

#include <unordered_set>
#include <vector>
#include <utility>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <assert.h>


Algorithm::Algorithm(const std::vector<edge> &edges,
                     const std::vector<weight> &weights) {

    assert(edges.size() == weights.size());

    edgeSize = edges.size();
    edgeArr.reset(new edge[edgeSize]);
    weightArr.reset(new weight[edgeSize]);
    for (size_t i = 0; i < edgeSize; i++) {
        *(edgeArr.get()+i) = edges.at(i);
        *(weightArr.get()+i)= weights.at(i);
    }
}

Algorithm::~Algorithm() {
    edgeArr.reset(nullptr);
    weightArr.reset(nullptr);
}

void Algorithm::MakeGraph() {
    myGraph = graph_t(edgeArr.get(), edgeArr.get() + edgeSize, weightArr.get(), edgeSize);
    boost::property_map<graph_t, boost::edge_weight_t>::type weightGraph =
            get(boost::edge_weight, myGraph);
}

std::vector<Algorithm::dotId> Algorithm::CalcRoute(const dotId &A,
                                                   const dotId &B) {
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

    //  идем в обратную сторону и запоминаем путь
    while ( current != pointA ) {
        path.push_back(current);
        current = pastWay[current];
    }
    path.push_back(pointA);  //  добавляем в конец точку с которой начинали

    std::vector<dotId> answer;

    //  в ответ записываем в правильном порядке
    std::vector<boost::graph_traits<graph_t>::vertex_descriptor >::
    reverse_iterator iter;
    for (iter = path.rbegin(); iter != path.rend(); ++iter)
        answer.push_back(*iter);

    return answer;
}

// new part
// возвращает id точек пути (включая исходную) и время для обхода
std::pair<std::vector<Algorithm::dotId>, size_t>
Algorithm::getRoute(Algorithm::dotId from, const size_t &pointsCount,
                    const size_t &time, const size_t &maxPlacesCount) {

    // только для дебага, время и кол-во мест проверяет сервер
    assert(time > 0);
    assert(maxPlacesCount > 1);

    size_t currentTime = 0;
    //  если пользователь тратит время на нахождение в точке, то следует
    //  добавлять это время кажыдй раз к currentTime
    std::pair<std::vector<Algorithm::dotId>, size_t> answer;
    std::unordered_set<dotId> visitedPints;
    visitedPints.insert(from); // пользователь был в точке из который начинает маршут
    answer.first.push_back(from); // дабавим начальную точку в путь

    // компаратор, чтобы сравнивать претендентов на место следущей точки
    auto comp = [](std::pair<dotId, size_t> a, std::pair<dotId, size_t> b)
    { return a.second < b.second; };

    while(true) {
        if (answer.first.size() >= maxPlacesCount + 1 ||  //  maxPlacesCount + 1 так как в ответе
            answer.first.size() > MAX_PLACES)       //  уже есть начальная точка
            break;

        //  получаем индекс, с которого находятся ребра от нашей вершины
        size_t startRibsIndexInEdges = (pointsCount-1) * from;

        //  получаем индекс, с которого находятся веса от нашей вершины
        size_t weightIndex = getWeightIndex(pointsCount, from, edgeArr.get()[startRibsIndexInEdges].second);

        //  возможные пути из точки
        std::set< std::pair<dotId, size_t>, decltype(comp) > nextPointPretendents(comp);

        // обходим места, куда можем попасть из данной точки
        for (size_t i = 0 ; i < pointsCount -1 ; i++, weightIndex++) {

            // не рассматриваем посещенные точки
            if ( visitedPints.find(edgeArr.get()[startRibsIndexInEdges + i].second) != visitedPints.end() )
                continue;

            // сохраняем допустимую точку
            nextPointPretendents.insert(std::make_pair(edgeArr.get()[startRibsIndexInEdges + i].second,
                                                       weightArr.get()[weightIndex]));
        }

        //  ничего не добавили => всё обошли
        if (nextPointPretendents.empty())
            break;

        //  получаем ближайшую точку
        std::pair<dotId, size_t> pretendent = *nextPointPretendents.begin();
        if (currentTime + pretendent.second <= time) {
            currentTime += pretendent.second;
            from = pretendent.first;
            answer.first.push_back(pretendent.first);
            visitedPints.insert(pretendent.first);
        } else
            //  ближайшая точка вне досягаемости за оставшееся время
            break;

    }
    answer.second = currentTime;
    return answer;
}


// delete this from algo
// данная функция будет вызываться как статическая из воркера-апи
long int Algorithm::getWeightIndex(const size_t &pointsCount, const size_t &from,
                                   const size_t &to) {

    if (from > pointsCount - 1 || to > pointsCount - 1 ||
        from == to)
        return -1; // return error

    if (from == 0)
        return to - 1;

    if (from < to)
        return ( (pointsCount -1) * from + to) - 1;

    return ( (pointsCount -1) * from + to);

}
