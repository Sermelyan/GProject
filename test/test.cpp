/*
 * Copyright 2019 <alex>
 */

#include <gtest/gtest.h>


#include <gtest/gtest.h>
#include <iostream>
#include "worker.hpp"
#include <queue>
#include <list>
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Return;
using ::testing::A;
using ::testing::ByRef;
using ::testing::Ref;
using ::testing::TypedEq;


TEST(Get, Get_from_queu) {
    Table a("../Attraction");
    std::vector<std::string> check = {"Красная_площадь","0"};
    std::vector<Point> res;
    a.SelectTag(check, res);
    std::vector<Point> resCheck = {{1,1},{5,13},{5,10},{4,8},{5,11}};
    ASSERT_EQ(res, resCheck);
}
//
TEST(Send, Send_to_queu) {
    Table a("../Attraction");
    std::vector<Point> res;
    a.SelectAll(res);
    std::vector<Point> resCheck = {{1, 1},{24, 78},{5, 13},{5, 10},{4, 8},{5, 11},
                                   {56, 65},{58, 65},{54, 63},{60, 65},{0, 0}};
    ASSERT_EQ(res, resCheck);
}

//TEST(Get_dots, Get_dots_from_DB) {
//    DataIn NotCorrect;
//    NotCorrect.UserID = -1;
//    DataOut NotCorrect2;
//    NotCorrect2.UserID = -1;
//    GQueue<DataIn> In(NotCorrect);
//    GQueue<DataOut> Out(NotCorrect2);
//    unsigned _t = 15;
//    unsigned _t2 = 200;
//    double a = 5;
//    size_t ttt = _t2;
//    DataIn forcheck({{"Красная_площадь"}, _t2, _t, {a,a}, {15,15}, 1});
//    std::string name = "../Attraction";
//    Worker w1(In, Out, name);
//    sleep (200);
////    std::cout << Out.popIfNotEmpty().RoutePoints;
//    ASSERT_EQ(1,1);
//}

//TEST(Get_dots2, Get_dots_from_DB2) {
//    MockQueue In;
//    MockQueue Out;
//    MockDatabase DB;
//    std::vector<Point> points;
//    Worker w1(In, Out, DB);
//    std::vector<std::string> filters {"1917"};
//    Limit lim;
//    for( int i = 0; i < 4; ++i ) {
//        lim.Point[i] = i * 100;
//    }
//    lim.Time = 256;
//    Data_in value(filters,1, lim);
//    w1.GetDotsFromDB(value, points);
//    std::vector<Point> check = {{46,34},{44,34}};
//    ASSERT_EQ(points,check);
//}
////
//TEST(Get_dots3, Get_dots_from_DB3) {
//    MockQueue In;
//    MockQueue Out;
//    MockDatabase DB;
//    std::vector<Point> points;
//    Worker w1(In, Out, DB);
//    std::vector<std::string> filters {"Достоевский"};
//    Limit lim;
//    for( int i = 0; i < 4; ++i ) {
//        lim.Point[i] = i * 100;
//    }
//    lim.Time = 256;
//    Data_in value(filters,1, lim);
//    w1.GetDotsFromDB(value, points);
//    std::vector<Point> check = {{46,34}};
//    ASSERT_EQ(points,check);
//}
//
//TEST(Get_dots4, NumberOfDots) {
//    MockQueue In;
//    MockQueue Out;
//    MockDatabase DB;
//    std::vector<Point> points;
//    Worker w1(In, Out, DB);
//    std::vector<std::string> filters {"Достоевский"};
//    Limit lim;
//    for( int i = 0; i < 4; ++i ) {
//        lim.Point[i] = i * 100;
//    }
//    lim.Time = 256;
//    lim.Number_of_dots = 0;
//    Data_in value(filters,1, lim);
//    w1.GetDotsFromDB(value, points);
//    std::vector<Point> check = {};
//    ASSERT_EQ(points,check);
//}
////
//TEST(Get_route, Get_route) {
//    MockQueue In;
//    MockQueue Out;
//    MockDatabase DB;
//    std::vector<Point> points {{1, 2},{1, 2},{1, 2}};
//    std::vector<std::vector<int>> ribs {{1, 2, 4},{1, 2}};
//    Worker w1(In, Out, DB);
//    std::list<Point> check;
//    std::list<Point> res;
//    w1.GetRoute(points, ribs, res);
//    ASSERT_EQ(check, res);
//}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

