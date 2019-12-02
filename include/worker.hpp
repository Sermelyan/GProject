/*
 * Copyright 2019 <Copyright Alex>
 */

#ifndef INCLUDE_WORKER_HPP_
#define INCLUDE_WORKER_HPP_

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream> // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! delete after tests

struct Point
{
    double X;
    double  Y;
};

namespace http = boost::beast::http;

class Worker {

public:  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! PRIVATE AFTER TESTS !!!!!!!!!!!!!!!!!!!!!!

    // this typedef get from Algorithm before merge we don't know about Algo
    typedef std::size_t dotId;
    typedef std::pair<dotId, dotId> edge;
    typedef std::size_t weight;

    // храним в этих векторах данные, которые получим из API
    std::vector<edge>  edges;
    std::vector<weight> weightArr;

    const size_t MAX_POINT_COUNT = 500;
    const size_t MIN_POINT_COUNT = 3;

    void GetRibsFromAPI(const std::vector<Point> &points);
    void getWeightFromPythonAPI(const std::string &jsonPoints, std::string &answer);
    std::string createJsonForSending(const std::vector<Point> &points);
    void setJsonAnswerInClass(const std::string &answer, const size_t &pointCount);

public:

    static long int getWeightIndex(const size_t &pointsCount, const size_t &from, const size_t &to);

};
#endif  //  INCLUDE_WORKER_HPP_

