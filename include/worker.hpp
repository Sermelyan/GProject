#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
//#include <gmock/gmock-spec-builders.h>
//#include <sqlite3.h>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "queu.hpp"

struct Point
{
    double X;
    double  Y;
};

struct Limit
{
    double Point[4];
    int Number_of_dots;
    int Time;
};

class Data_in
{
private:
    std::vector<std::string> Filters;
    int User_id;
    Limit limits;
public:
    Data_in();
    Data_in(const std::vector<std::string> &f, const int id, const Limit l);
    ~Data_in();
    Limit Get_Limit() const;
    void Get_Filters(std::vector<std::string> &filtr) const;
    void Set_Limit(const Limit value);
    void Set_Filters(const std::vector<std::string> &filtr);
    Data_in operator=(const Data_in &Reight);
};

bool operator== (const Data_in left, const Data_in right);//{
//    return true;
//}

class Data_out
{
private:
    std::vector<Point> Points;
    int User_id;
    int Time;
public:
    Data_out();
    Data_out(const std::vector<Point> &points, const int id, const int time);
    ~Data_out();
    void Get_Points(std::vector<std::pair<double, double >> &point) const;
    int Get_Id() const;
    int Get_Time() const;
    void Set_Id(const int id);
    void Set_Time(const int time);
    void Set_Points(std::vector<std::pair<double, double >> &filtr);
    Data_out operator=(testing::internal::OnCallSpec<Data_out(void)> Reight);

    void Get_Points(std::vector<std::pair<double, double>> point) const;
};

bool operator== (const Data_out left, const Data_out right);
bool operator==(const Point left, const Point right);

class Sqlite {
// private:
//    sqllite3* DB;
// public:
//    void query(const std::string& query);
//    void exec(const std::string& query);
//    bool isConnected() const;

//     Sqlite(const std::string& filename);
//    virtual ~Sqlite();
};

class Worker
{
private:
    Data_in request;
    Sqlite DB;
    Queue &In;
    Queue &Out;
    Data_in GetFromQueueIn();
    void SendToQueueOut(const Data_out &value);
    void GetDotsFromDB(const Data_in &value, std::vector<Point> &points);
    void GetRibsFromAPI(const std::vector<Point> &points, std::vector<std::vector<double >> &ribs);
    void GetRoute(const std::vector<Point> &points, const std::vector<std::vector<int>> &ribs, std::list<Point> &res);
    void WorkerProcess();

public:
    Worker(Queue &in, Queue &out, Sqlite &DB);
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
