/*
 * Copyright 2019 <Copyright Owner>
 */

#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

#include <vector>
#include <list>

struct Point
{
    double X;
    double  Y;
};

class Worker
{
private:

    // this typedef get from Algorithm
    typedef std::size_t dotId;
    typedef std::pair<dotId, dotId> edge;
    typedef std::size_t weight;


    std::vector<edge>  edges;
    std::vector<weight> weightArr;

    void GetRibsFromAPI(const std::vector<Point> &points);

    weight getWeightFromAPI(const Point &A, const Point &B);

    void GetRoute(const std::vector<Point> &points,
            const std::vector<std::vector<int>> &ribs, std::list<Point> &res);




};
#endif  //  INCLUDE_WORKER_HPP_
