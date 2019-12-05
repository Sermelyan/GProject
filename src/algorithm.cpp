/*
 * Copyright 2019 <Alex>
 */

#include "algorithm.hpp"

Algorithm::Algorithm(const std::vector<edge> &edges,
                     const std::vector<weight> &weights) {

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

struct PointNode {
    Algorithm::dotId pointId;
    std::vector<PointNode *> ptrNodeArr;
};


void Algorithm::recBFS(const Algorithm::dotId &from, const size_t &pointsCount,
            const size_t &time, size_t &currentTime, std::vector<dotId> &visitedPints,
            std::pair<std::vector<Algorithm::dotId>, size_t> &answer) {

    if (answer.first.size() > MAX_RECURSION)
        return;

    visitedPints.push_back(from);

    size_t startRibsIndexInEdges = (pointsCount-1) * from;
    std::cout<<"INDEX START:"<< startRibsIndexInEdges <<"\n";

    std::cout <<"FROM:"<< edgeArr[startRibsIndexInEdges].first <<"(->"<<
              edgeArr[startRibsIndexInEdges].second<<")\n";

    size_t weightIndex = getWeightIndex(pointsCount, from, 0);
    for (size_t i = 0 ; i < pointsCount -1 ; i++) {  // не рассматривать посещенные
        std::cout<<"WEIGHT INDEX:"<<weightIndex<<"\n";
        std::cout<<"WAY WEIGHT("<<edgeArr[startRibsIndexInEdges].first<<"->"
                 <<edgeArr[startRibsIndexInEdges + i].second<<"):"<<weightArr[weightIndex] <<"\n";
        weightIndex++;
    }
    // выбрали нужный
    // и после цикла вызвали
}


// возвращает айди точек по пути и время для обхода
std::pair<std::vector<Algorithm::dotId>, size_t>
Algorithm::getRoute(const Algorithm::dotId &from, const size_t &pointsCount, const size_t &time) {

    assert(time > 0);

    // не создавать граф =)
    // создать массив посещенных
    // функция поиска ближайшего

    size_t currentTime = 0;
    //  если пользователь тратит время на нахождение в точке, то следует
    //  добавлять это время кажыдй раз к currentTime (тут и в поиске)
    std::pair<std::vector<Algorithm::dotId>, size_t> answer;
    std::vector<dotId> visitedPints;

    recBFS(from,pointsCount,time,currentTime, visitedPints, answer);


    return answer;
}



// delete this from algo
long int Algorithm::getWeightIndex(const size_t &pointsCount, const size_t &from, const size_t &to){

    if (from > pointsCount - 1 || to > pointsCount - 1 ||
        from == to)
        return -1; // return error

    if (from == 0)
        return to - 1;

    if (from < to)
        return ( (pointsCount -1) * from + to) - 1;

    return ( (pointsCount -1) * from + to);

}
