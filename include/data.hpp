/*
 * Copyright 2019 <Copyright Owner>
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#ifndef INCLUDE_DATA_HPP_
#define INCLUDE_DATA_HPP_

static const unsigned MIN_IN_HOUR = 60;

typedef std::pair<double, double> Point;
typedef std::vector<Point> Points;
typedef std::vector<std::string> Filters;

struct DataIn {
    Filters FilterList;
    unsigned TimeLimit;
    unsigned MaxDots;
    Point StartPoint;
    Point EndPoint;
    int UserID;
    DataIn() {}
    DataIn(::Filters _f, unsigned _t, unsigned _md, Point _sp, Point _ep,
           int _u)
        : FilterList(std::move(_f)),
          TimeLimit(_t),
          MaxDots(_md),
          StartPoint(std::move(_sp)),
          EndPoint(std::move(_ep)),
          UserID(_u) {}
};

struct DataOut {
    Points RoutePoints;
    unsigned MaxTime;
    int UserID;
    DataOut() {}
    DataOut(Points _p, unsigned _t, int _u)
        : RoutePoints(std::move(_p)), MaxTime(_t), UserID(_u) {}
};

#endif  //  INCLUDE_DATA_HPP_
