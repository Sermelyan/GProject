//
// Created by alex on 21/12/2019.
//

#include <iostream>
#include "Test.hpp"

#define DEBUG 1

int main(int argc, char** argv) {


    std::cout<<"\n  START MAIN!  \n";
    if (DEBUG) {
        testing::InitGoogleTest(&argc, argv);
        allTest();

    }



    std::cout<<"\n  END MAIN!  \n";

    return 0;
}