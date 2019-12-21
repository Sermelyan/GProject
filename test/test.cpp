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

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

