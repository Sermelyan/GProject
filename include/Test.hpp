//
// Created by alex on 21/12/2019.
//

#ifndef GPROJECT_TEST_HPP
#define GPROJECT_TEST_HPP

#include <gtest/gtest.h>
#include "worker.hpp"
#include <iostream>
#include "queue.hpp"
#include <thread>
#include "algorithm.hpp"
#include <string>  // only for demonstrate display places names

int allTest();

template <class T>
void generateArr(T *arr, size_t arrSize);
template <class T>
void generateRandomArr(T *arr, size_t arrSize);
void threadPush(GQueue<long int> * gq, long int *arr, size_t size );
void threadPop(GQueue<long int> * gq, long int *arr, size_t size );


#endif //GPROJECT_TEST_HPP
