#include <string>
#include <utility>
#include <vector>
#include <map>
//#include "sqlite-autoconf-3300100/sqlite3.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "queue.hpp"
#include "algorithm.hpp"
#include <thread>
#include "Data.hpp"
#include <fstream>

#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

struct Limit
{
    double Point[4];
    int Number_of_dots;
    int Time;
};

struct Line {
    int id;
    std::string Name;
    double x;
    double y;
    std::vector<std::string>Tags;
    Line(int ID, std::string n, double X, double Y, std::vector<std::string>T): id(ID), Name(std::move(n)), x(X), y(Y), Tags(std::move(T)){};
    Line(): id(0), Name("name"), x(0), y(0), Tags({}){};
};

class Table {
 public:
    std::ifstream file;
    std::vector<Line> table;
 public:
    void SelectTag(std::vector<std::string> values, std::vector<Point>& res);
    void SelectAll(std::vector<Point>& res);
    Table(std::string filename);
    virtual ~Table();
};

class Worker
{
private:
    DataIn request;
    Table DB;
    GQueue<DataIn> &In;
    GQueue<DataOut> &Out;
    bool Stop;
    std::vector<std::pair<Algorithm::dotId, Algorithm::dotId>>  edges; // вектор из 2 точек
    std::vector<Algorithm::dotId > weightArr; // вес ребра
    std::thread WProces;
    DataIn GetFromQueueIn();
    void SendToQueueOut(const DataOut &value);
    void GetDotsFromDB(const DataIn &value, std::vector<Point> &points);
    void GetRibsFromAPI(const std::vector<Point> &points);
//    void GetRoute(std::vector<Algorithm::dotId>  edges, std::vector<Algorithm::weight> weightArr,
//            std::pair<std::vector<int>, size_t> &res, size_t num_dots, DataIn value);
    void FinalPoints(std::vector<Point> &points, const std::pair<std::vector<int>, size_t> &res);
    void WorkerProcess();

public:
    Worker(GQueue<DataIn> &in, GQueue<DataOut> &out, std::string DBName);
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
