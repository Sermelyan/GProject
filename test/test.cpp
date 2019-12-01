/*
 * Copyright 2019 <Copyright Owner>
 */

#include <gtest/gtest.h>
#include <iostream>

TEST(hello, world) {
    std::cout << "Hello, world!" << std::endl;
}




int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
