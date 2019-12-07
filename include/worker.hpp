#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

#include <string>
#include <vector>
#include <map>
#include "sqlite-autoconf-3300100/sqlite3.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "queu.hpp"
#include "algorithm.hpp"
#include <thread>
static const unsigned MIN_IN_HOUR = 60;

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

struct Limit
{
    double Point[4];
    int Number_of_dots;
    int Time;
};

class Sqlite {
 private:
    sqlite3* DB = 0;
    char *zErrMsg = 0;
    int rc;
 public:
    void Select(const char *sql, std::vector<Point> res);
    Sqlite(const char * filename);
    virtual ~Sqlite();
};

class Worker
{
private:
    DataIn request;
    Sqlite DB;
    GQueue<DataIn> &In;
    GQueue<DataOut> &Out;
    bool Stop;
    std::vector<std::pair<size_t,size_t>>  edges; // вектор из 2 точек
    std::vector<size_t> weightArr; // вес ребра
    std::thread WProces;
    DataIn GetFromQueueIn();
    void SendToQueueOut(const DataOut &value);
    void GetDotsFromDB(const DataIn &value, std::vector<Point> &points);
    void GetRibsFromAPI(const std::vector<Point> &points);
    void GetRoute(std::vector<std::pair<size_t,size_t>>  edges, std::vector<size_t> weightArr,
            std::pair<std::vector<int>, size_t> &res, size_t num_dots, DataIn value);
    void FinalPoints(std::vector<Point> &points, const std::pair<std::vector<int>, size_t> &res);
    void WorkerProcess();

public:
    Worker(GQueue<DataIn> &in, GQueue<DataOut> &out, const char * DBName);
    ~Worker();
    void Kill();
    FRIEND_TEST(Get, Get_from_queu);
    FRIEND_TEST(Send, Send_to_queu);
    FRIEND_TEST(Get_dots, Get_dots_from_DB);
    FRIEND_TEST(Get_dots2, Get_dots_from_DB2);
    FRIEND_TEST(Get_dots3, Get_dots_from_DB3);
    FRIEND_TEST(Get_dots4, NumberOfDots);
    FRIEND_TEST(Get_route, Get_route);
};
#endif  //  INCLUDE_WORKER_HPP_
