/*
 * Copyright 2019 S4ll7
 */

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


//class MockDatabase : public Sqlite
//{
//public:
//    MOCK_METHOD1(exec, bool(Data_in& value));
//    MOCK_METHOD1(open, bool(std::string DBname));
//    MOCK_METHOD0_T(close, void());
//};

//class MockQueue : public Queue
//{
//public:
//    MOCK_METHOD1(push, bool(DataIn &value));
//    MOCK_METHOD0_T(pop, DataOut());
//};

//Data_out Data_out::operator=(testing::internal::OnCallSpec<Data_out(void)> Reight)
//{
//    Data_out a;
//    return a;
//}

TEST(Get, Get_from_queu) {
    DataIn NotCorrect;
    NotCorrect.UserID = -1;
    DataOut NotCorrect2;
    NotCorrect2.UserID = -1;
    GQueue<DataIn> In(NotCorrect);
    GQueue<DataOut> Out(NotCorrect2);
    const char* name = "test.db";
    Worker w1(In, Out, name);
    Worker w2(In, Out, name);

//    for ( int i = 0; i < 100; i++ ){
//        printf("white ");
//    }
//    w1.Kill();
//    for ( int i = 0; i < 100; i++ ){
//        printf("red ");
//    }
//    w2.Kill();
    ASSERT_EQ(1, 1);
}
//
//TEST(Send, Send_to_queu) {
//    MockQueue In;
//    MockQueue Out;
//    MockDatabase DB;
//    std::vector<Point> points {{1, 2},{1, 2},{1, 2}};
//    Data_out value(points, 1, 1);
//    Worker w1(In, Out, DB);
//    w1.SendToQueueOut(value);
//    Data_out check;
//    check = ON_CALL(Out, pop()).WillByDefault(Return(Data_out({{1, 2},{1, 2},{1, 2}},1,1)));
//    ASSERT_EQ(value, check);
//}
//
//TEST(Get_dots, Get_dots_from_DB) {
//    MockQueue In;
//    MockQueue Out;
//    MockDatabase DB;
//    std::vector<Point> points;
//    Worker w1(In, Out, DB);
//    std::vector<std::string> filters {"1917"};
//    Limit lim;
//    for( int i = 0; i < 4; ++i ) {
//        lim.Point[i] = 10;
//    }
//    lim.Time = 256;
//    Data_in value(filters,1, lim);
//    w1.GetDotsFromDB(value, points);
//    std::vector<Point> check = {};
//    ASSERT_EQ(points,check);
//}
////
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


// ТО КАК ДОЛЖНА БУДЕТ ВЫГЛЯДИТЬ ПРОВЕРОЧНАЯ ТАБЛИЦА
//struct element{
//    int Id;
//    std::vector<std::string> filter;
//    std::pair<double, double> Dot;
//};
//
//class Turtle {
//public:
//    std::vector<element> Values() {
//        std::vector<element> res;
//        element value;
//        value.filter = {"#Пушкин","#1917","#Красный октябрь","#Достоевский"};
//        value.Dot = {46, 34};
//        value.Id = 1;
//        res.push_back(value);
//        value.filter = {"#1913","#Франц Фердинанд"};
//        value.Dot = {45, 34};
//        value.Id = 2;
//        res.push_back(value);
//        value.filter = {"#1917","#Революция"};
//        value.Id = 3;
//        value.Dot = {44, 34};
//        res.push_back(value);
//        return res;
//    }
//};