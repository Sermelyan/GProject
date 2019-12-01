/*
 * Copyright 2019 <Copyright Owner>
 */

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#ifndef INCLUDE_DATA_HPP_
#define INCLUDE_DATA_HPP_

static const unsigned SEC_IN_HOUR = 3600;
static const unsigned SEC_IN_MIN = 60;

typedef std::pair<double, double> point;
typedef std::vector<point> points;
typedef std::vector<std::string> filters;

struct In {
    filters Filters;
    unsigned TimeLimit;
    unsigned MaxDots;
    point StartPoint;
    int UserID;
    In() {}
    In(const filters &_f, unsigned _t, const point &_p, int _u)
        : Filters(_f), TimeLimit(_t), StartPoint(_p), UserID(_u) {}
};

struct Out {
    points RoutePoints;
    unsigned MaxTime;
    int UserID;
    Out() {}
    Out(const points &_p, unsigned _t, int _u)
        : RoutePoints(_p), MaxTime(_t), UserID(_u) {}
};

#endif  //  INCLUDE_DATA_HPP_
