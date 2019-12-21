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

TEST(Get_dots, Get_dots_from_DB) {
    DataIn NotCorrect;
    NotCorrect.UserID = -1;
    DataOut NotCorrect2;
    NotCorrect2.UserID = -1;
    GQueue<DataIn> In(NotCorrect);
    GQueue<DataOut> Out(NotCorrect2);
    DataIn forcheck;
    forcheck.FilterList = {{"dfgvd"}};
    Worker w1(In, Out, "../Attraction");
    w1.Kill();
    std::vector<Point> a;
    std::vector<Point> check = {};
    w1.GetDotsFromDB(forcheck,a);
    ASSERT_EQ(a, check);
}

TEST(Get_dots2, Get_dots_from_DB2) {
    DataIn NotCorrect;
    NotCorrect.UserID = -1;
    DataOut NotCorrect2;
    NotCorrect2.UserID = -1;
    GQueue<DataIn> In(NotCorrect);
    GQueue<DataOut> Out(NotCorrect2);
    DataIn forcheck;
    forcheck.FilterList = {{"Красная_площадь"}};
    Worker w1(In, Out, "../Attraction");
    w1.Kill();
    std::vector<Point> a;
    std::vector<Point> check = {{5,13},{5,10},{4,8},{5, 11}};
    w1.GetDotsFromDB(forcheck,a);
    ASSERT_EQ(a, check);
}
////
TEST(Get_dots3, Get_dots_from_DB3) {
    DataIn NotCorrect;
    NotCorrect.UserID = -1;
    DataOut NotCorrect2;
    NotCorrect2.UserID = -1;
    GQueue<DataIn> In(NotCorrect);
    GQueue<DataOut> Out(NotCorrect2);
    DataIn forcheck;
    forcheck.FilterList = {{"Красная_площадь"},{"История"}};
    Worker w1(In, Out, "../Attraction");
    w1.Kill();
    std::vector<Point> a;
//    for ( auto i : a ) {
//        std::cout<<i.first << i.second << std::endl;
//    }
    std::vector<Point> check = { {5, 13}, {5, 10}, {4, 8}, {5, 11}, {58, 65},
                                 {54, 63}, {60, 65} };
    w1.GetDotsFromDB(forcheck,a);
    ASSERT_EQ(a, check);
}
//
TEST(Get_dots4, NumberOfDots) {
    DataIn NotCorrect;
    NotCorrect.UserID = -1;
    DataOut NotCorrect2;
    NotCorrect2.UserID = -1;
    GQueue<DataIn> In(NotCorrect);
    GQueue<DataOut> Out(NotCorrect2);
    DataIn forcheck;
    forcheck.FilterList = {{"История"}};
    Worker w1(In, Out, "../Attraction");
    w1.Kill();
    In.push(forcheck);
    DataIn a;
    a.FilterList = w1.GetFromQueueIn().FilterList;
    ASSERT_EQ(a.FilterList, forcheck.FilterList);
}

TEST(Get_dots5, NumberOfDots) {
    DataIn NotCorrect;
    NotCorrect.UserID = -1;
    DataOut NotCorrect2;
    NotCorrect2.UserID = -1;
    GQueue<DataIn> In(NotCorrect);
    GQueue<DataOut> Out(NotCorrect2);
    Worker w1(In, Out, "../Attraction");
    w1.Kill();
    std::vector<Point> points = {{1,2},{3,4},{5,6},{7,8},{9,10},{11,11}};
    std::pair<std::vector<Algorithm::dotId >, size_t> res = {{1,3,5},5};
    std::vector<Point> Result = {{3, 4}, {7, 8}, {11, 11}};
    w1.FinalPoints(points,res);
    ASSERT_EQ(points, Result);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

