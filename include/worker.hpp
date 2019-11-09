/*
 * Copyright 2019 S4lly
 */

#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <list>
// #include <sqlite3.h>

struct Limit {
    double Point[4];
    int Time;
};

class Data_in {
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

bool operator== (const Data_in left, const Data_in right);

class Data_out {
 private:
    std::vector<std::pair<double, double >> Points;
    int User_id;
    int Time;
 public:
    Data_out();
    Data_out(const std::vector<std::pair<double, double >> &points,
            const int id, const int time);
    ~Data_out();
    void Get_Points(std::vector<std::pair<double, double >> &point) const;
    int Get_Id() const;
    int Get_Time() const;
    void Set_Id(const int id);
    void Set_Time(const int time);
    void Set_Points(std::vector<std::pair<double, double >> &filtr);
    Data_out operator=(const Data_out &Reight);

    void Get_Points(std::vector<std::pair<double, double>> point) const;
};

bool operator== (const Data_out left, const Data_out right);

class Worker {
 private:
    Data_in request;
    std::queue<Data_out> *Out;
    std::queue<Data_in> *In;
    // sqlite3 *Connection;
    // GQueue &In;
    // GQueue &Out;
 public:
    Worker();
    Worker(std::queue<Data_in> *in, std::queue<Data_out> *out) {
        In = in;
        Out = out;
    }
    // Worker(sqlite3 *conect, GQueue &In,GQueue &In);
    ~Worker();
    Data_in GetFromQueu();
    void SendToQueu(const Data_out value);
    void GetDotsFromDB(std::vector<std::pair<double, double>> &points,
            Data_in value);
    void GetRibsFromAPI(std::vector<std::pair<double, double>> &points,
            std::vector<std::vector<double >> &ribs);
    void GetRoute(std::vector<std::pair<double, double>> &points,
            std::vector<std::vector<int>> &ribs,
            std::list<std::pair<double, double>> res);
    void Check();

};

#endif  //  INCLUDE_WORKER_HPP_
