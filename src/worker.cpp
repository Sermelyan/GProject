/*
 * Copyright 2019 <Copyright Alex>
 */

#include "worker.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream> // deleeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeteeeeeeeeee
#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace http = boost::beast::http;

void Worker::GetRibsFromAPI(const std::vector<Point> &points ) {

    const size_t pointCount = points.size();
    if (points.size() < 3)
        return;

    //  size_t ribsCount = ( pointCount * (pointCount - 1) / 2 ) ;  //  need * 2 тк двунаправленный

    //  только обходом n * n получим все (вербро туда-обратно)

    /*
    edge edg;
    weight edgeWeight;
    for (size_t i = 0 ; i < pointCount ; i++)
        for (size_t j = 1 ; j < pointCount ; j++) {

            if (i == j) continue;  //  не нужен путь от А до пункта А

            edg = std::make_pair(i, j);
            edgeWeight = getWeightFromAPI(points.at(i), points.at(i));
            this->edges.push_back(edg);
            this->weightArr.push_back(edgeWeight);
        }
        */

    boost::property_tree::ptree rootTree;

    boost::property_tree::ptree coordinates;
    boost::property_tree::ptree coordinate;
    for (size_t i = 0 ; i < points.size(); i++) {  //for (auto &point : points)
        coordinate.put("X", points.at(i).X);
        coordinate.put("Y", points.at(i).Y);
        std::string id = std::to_string(i);
        coordinates.add_child( id, coordinate);
    }

    rootTree.add_child("coordinates", coordinates);
    std::stringstream ss;
    boost::property_tree::write_json(ss, rootTree);
    const std::string result = ss.str();
    std::cout<<result<<"\n";

    // sudo apt-get install python3
    // pip3 install flask
    // python3 ./main.py

    std::string answer;
    getWeightFromPythonAPI(result, answer);

    std::cout<<"===================ANSWER FROM PYTHON==============\n";
    std::cout<<answer;
    std::cout<<"===================END ANSWER FROM PYTHON==============\n";

    // распоковать json
    // записать в переменные класса
    // написать тесты
    // написать хедр нового алгоритма
    // написать тесты нового алгоритма
    // написать новый алгоритм

}




void Worker::getWeightFromPythonAPI(const std::string &jsonPoints, std::string &answer) {

    const std::string host = "127.0.0.1";
    const std::string target = "/api";

    // I/O контекст, необходимый для всех I/O операций
    boost::asio::io_context ioc;

    // Resolver для определения endpoint'ов
    boost::asio::ip::tcp::resolver resolver(ioc);
    // Tcp сокет, использующейся для соединения
    boost::asio::ip::tcp::socket socket(ioc);

    // Резолвим адрес и устанавливаем соединение
    boost::asio::connect(socket, resolver.resolve(host, "5000"));

    // Дальше необходимо создать HTTP GET реквест с указанием таргета
    http::request<http::string_body> req(http::verb::post, target, 11);

    // Задаём поля HTTP заголовка
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::accept_charset, "utf-8");

    req.content_length(jsonPoints.size());
    req.keep_alive(req.keep_alive());

    req.body() = jsonPoints;

    req.prepare_payload();

    // Отправляем реквест через приконекченный сокет
    http::write(socket, req);

    // Часть, отвечающая за чтение респонса
    std::stringstream answerStream;
    {
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(socket, buffer, res);
        answerStream << res;
    }
    // Закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    // Стираем заголовки из ответа
    answer = answerStream.str();
    size_t startJson = answer.find('{');
    answer.erase(0, startJson);
}


void Worker::GetRoute(const std::vector<Point> &points, const std::vector<std::vector<int>> &ribs,
                      std::list<Point> &res){

}