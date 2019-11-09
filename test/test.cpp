/*
 * Copyright 2019 <Copyright Owner>
 */

#include <gtest/gtest.h>
#include <iostream>
#include "worker.hpp"
#include <queue>
#include <list>
//#include "queu.hpp"
#include "gmock/gmock.h"


struct element{
    int Id;
    std::vector<std::string> filter;
    std::pair<double, double> Dot;
};

class Turtle {
public:
    std::vector<element> Values() {
        std::vector<element> res;
        element value;
        value.filter = {"#Пушкин","#1917","#Красный октябрь","#Достоевский"};
        value.Dot = {46, 34};
        value.Id = 1;
        res.push_back(value);
        value.filter = {"#1913","#Франц Фердинанд"};
        value.Dot = {45, 34};
        value.Id = 2;
        res.push_back(value);
        value.filter = {"#1917","#Революция"};
        value.Id = 3;
        value.Dot = {44, 34};
        res.push_back(value);
        return res;
    }
};




TEST(Get, Get_from_queu) {
//    GQueue a;
    std::queue<Data_in> Q1;
    std::queue<Data_out> Q2;
    std::vector<std::string> filters {"#a","#b","#c"};
    Limit lim;
    for( int i = 0; i < 4; ++i ) {
        lim.Point[i] = i;
    }
    lim.Time = 256;
    Data_in value(filters,1, lim);
    Q1.push(value);
    Worker w1(&Q1,&Q2);
    Data_in check;
    check = w1.GetFromQueu();
    ASSERT_EQ(value,check);
}

TEST(Send, Send_to_queu) {
    std::queue<Data_out> Q2;
    std::queue<Data_in> Q1;
    std::vector<std::pair<double , double >> points {{1, 2},{1, 2},{1, 2}};
    Data_out value(points, 1, 1);
    Worker w1(&Q1,&Q2);
    w1.SendToQueu(value);
    Data_out check;
    check = Q2.front();
    ASSERT_EQ(value,check);
}

TEST(Get_dots, Get_dots_from_DB) {
//    MockDatabase database;
    std::queue<Data_out> Q2;
    std::queue<Data_in> Q1;
    std::vector<std::pair<double , double >> points;
    Worker w1(&Q1,&Q2);
    std::vector<std::string> filters {"1917"};
    Limit lim;
    for( int i = 0; i < 4; ++i ) {
        lim.Point[i] = 10;
    }
    lim.Time = 256;
    Data_in value(filters,1, lim);
    w1.GetDotsFromDB(points, value);
    std::vector<std::pair<double , double >> check = {};
    ASSERT_EQ(points,check);
}

TEST(Get_dots2, Get_dots_from_DB2) {
//    MockDatabase database;
    std::queue<Data_out> Q2;
    std::queue<Data_in> Q1;
    std::vector<std::pair<double , double >> points;
    Worker w1(&Q1,&Q2);
    std::vector<std::string> filters {"1917"};
    Limit lim;
    for( int i = 0; i < 4; ++i ) {
        lim.Point[i] = i * 100;
    }
    lim.Time = 256;
    Data_in value(filters,1, lim);
    w1.GetDotsFromDB(points, value);
    std::vector<std::pair<double , double >> check = {{46,34},{44,34}};
    ASSERT_EQ(points,check);
}

TEST(Get_dots3, Get_dots_from_DB3) {
//    MockDatabase database;
    std::queue<Data_out> Q2;
    std::queue<Data_in> Q1;
    std::vector<std::pair<double , double >> points;
    Worker w1(&Q1,&Q2);
    std::vector<std::string> filters {"Достоевский"};
    Limit lim;
    for( int i = 0; i < 4; ++i ) {
        lim.Point[i] = i * 100;
    }
    lim.Time = 256;
    Data_in value(filters,1, lim);
    w1.GetDotsFromDB(points, value);
    std::vector<std::pair<double , double >> check = {{46,34}};
    ASSERT_EQ(points,check);
}

TEST(Get_route, Get_route) {
    std::queue<Data_out> Q2;
    std::queue<Data_in> Q1;
    std::vector<std::pair<double , double >> points {{1, 2},{1, 2},{1, 2}};
    std::vector<std::vector<int>> ribs {{1, 2, 4},{1, 2}};
    Worker w1(&Q1,&Q2);
    std::list<std::pair<double, double>> check;
    std::list<std::pair<double, double>> res;
    w1.GetRoute(points, ribs, res);
    ASSERT_EQ(check, res);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
