/*
 * Copyright 2019 <Copyright Owner>
 */

#include <gtest/gtest.h>
#include <iostream>
#include "worker.hpp"


TEST(hello, world) {
    std::cout << "Hello, world!" << std::endl;
    Worker worker;
    std::vector<Point> points;
    points.push_back( Point{4.23,3.2} );
    points.push_back( Point{3.23,7.2} );
    points.push_back( Point{-2.23,16.2} );
    points.push_back( Point{1.23,-3.2} );
    points.push_back( Point{15.23,13.2} );

    worker.GetRibsFromAPI(points);
    return;
}




int main(int argc, char** argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
