/*
 * Copyright 2019 <Copyright Alex>
 */

#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

#include <vector>
#include <list>
#include <string>
#include <sstream>

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

    // void GetRibsFromAPI(const std::vector<Point> &points);
    //std::string getWeightFromPythonAPI(const std::string &jsonPoints);

    void GetRoute(const std::vector<Point> &points,
            const std::vector<std::vector<int>> &ribs, std::list<Point> &res);

public: //  delete and uncomment 2 fun. it's only for tests
        void GetRibsFromAPI(const std::vector<Point> &points);
        void getWeightFromPythonAPI(const std::string &jsonPoints, std::string &answer);

};
#endif  //  INCLUDE_WORKER_HPP_
