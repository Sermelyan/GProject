/*
 * Copyright 2019 <Copyright Owner>
 */

#include "worker.hpp"


void Worker::GetRibsFromAPI(const std::vector<Point> &points ) {

    const size_t pointCount = points.size();
    if (points.size() < 4)
        return;

    //  size_t ribsCount = ( pointCount * (pointCount - 1) / 2 ) ;  //  need * 2 тк двунаправленный

    //  только обходом n * n получим все (вербро туда-обратно)
    edge edg;
    weight edgeWeight;
    for (size_t i = 0 ; i < pointCount ; i++)
        for (size_t j = 1 ; j < pointCount ; j++) {

            if (i == j) continue;  //  не нужен путь от А до пункта А

            edg = std::make_pair(i, j);
            edgeWeight = getWeightFromAPI(points.at(i), points.at(i));
            this->edges.push_back(edg);
            this->weightArr.push_back(edgeWeight);
        }
}


Worker::weight  Worker::getWeightFromAPI(const Point &A, const Point &B){

    return 5;
}




void Worker::GetRoute(const std::vector<Point> &points, const std::vector<std::vector<int>> &ribs,
                      std::list<Point> &res){

}