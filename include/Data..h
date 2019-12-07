//
// Created by s4lly on 07.12.2019.
//

#ifndef GPROJECT_DATA_H
#define GPROJECT_DATA_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#ifndef INCLUDE_DATA_HPP_
#define INCLUDE_DATA_HPP_

static const unsigned MIN_IN_HOUR = 60;

//typedef std::pair<double, double> Point;
struct Point
{
    int id;
    std::vector<std::string> tags;
    double X;
    double  Y;
    bool operator== (Point &R){
        if (this->id == R.id && this->tags == R.tags && this->X == R.X && this->Y == R.Y){
            return true;
        }
        return false;
    }
};
typedef std::vector<Point> Points;
typedef std::vector<std::string> Filters;

struct DataIn
{
    Filters FilterList;
    unsigned TimeLimit;
    unsigned MaxDots;
    Point StartPoint;
    Point EndPoint;
    int UserID;
    DataIn() {}
    DataIn(::Filters _f, unsigned _t, unsigned _md, Point _sp, Point _ep, int _u)
            : FilterList(_f),
              TimeLimit(_t),
              MaxDots(_md),
              StartPoint(_sp),
              EndPoint(_ep),
              UserID(_u) {}

    bool operator== (DataIn &Right){
        if (this->EndPoint == Right.EndPoint && this->StartPoint == Right.StartPoint && this->FilterList == Right.FilterList &&
                this->UserID == Right.UserID && this->MaxDots == Right.MaxDots && this->TimeLimit == Right.TimeLimit) {
            return true;
        }
        return false;
    }
};


struct DataOut {
    Points RoutePoints;
    unsigned MaxTime;
    int UserID;
    DataOut() {}
    DataOut(const Points& _p, unsigned _t, int _u)
            : RoutePoints(_p), MaxTime(_t), UserID(_u) {}
};

#endif  //  INCLUDE_DATA_HPP_

#endif //GPROJECT_DATA_H
