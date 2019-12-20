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

#include <iostream>

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

    std::string host = "127.0.0.1";
    std::string target = "/api";
    void setHostTarget(const std::string &host, const std::string &target);
    bool GetRibsFromAPI(const std::vector<Point> &points);
    bool getWeightFromPythonAPI(const std::string &jsonPoints, std::string &answer);
    std::string createJsonForSending(const std::vector<Point> &points);
    void setJsonAnswerInClass(const std::string &answer, const size_t &pointCount);

public:

    static long int getWeightIndex(const size_t &pointsCount, const size_t &from, const size_t &to);

};
#endif  //  INCLUDE_WORKER_HPP_

