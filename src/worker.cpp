/*
 * Copyright 2019 <Copyright Alex>
 */

#include "worker.hpp"


void Worker::GetRibsFromAPI(const std::vector<Point> &points ) {

    const size_t pointCount = points.size();
    if (pointCount < MIN_POINT_COUNT || pointCount > MAX_POINT_COUNT)
        return;

    // const size_t ribsCount = ( pointCount * (pointCount - 1) / 2 ) * 2;  //  теория

    const std::string jsonForSending = createJsonForSending(points);

    // std::cout << jsonForSending << "\n";

    std::string answer;
    getWeightFromPythonAPI(jsonForSending, answer);

    //std::cout<<"===================ANSWER FROM PYTHON==============\n";
    //std::cout<<answer;
    //std::cout<<"===================END ANSWER FROM PYTHON==============\n";

    setJsonAnswerInClass(answer, pointCount);

    //size_t ribsCount = ( pointCount * (pointCount - 1) / 2 ) * 2 ;
    //std::cout<<"REAL SIZE:"<<  edges.size()<<" =="<< weightArr.size() <<"\n=============================\n";
    //std::cout<<"My theoretical SIZE:"<<  ribsCount <<"\n=============================\n";

    // написать тесты
    // написать хедр нового алгоритма
    // написать тесты нового алгоритма
    // написать новый алгоритм

}


std::string Worker::createJsonForSending(const std::vector<Point> &points) {

    boost::property_tree::ptree rootJsonTree;
    boost::property_tree::ptree coordinates;
    boost::property_tree::ptree coordinate;

    for (size_t i = 0 ; i < points.size() ; i++) {
        coordinate.put("X", points.at(i).X);
        coordinate.put("Y", points.at(i).Y);
        // id координаты = её место в принимающем векторе
        std::string id = std::to_string(i);
        coordinates.add_child( id, coordinate);
    }

    rootJsonTree.add_child("coordinates", coordinates);
    std::stringstream jsonStream;
    boost::property_tree::write_json(jsonStream, rootJsonTree);

    return jsonStream.str();
}


void Worker::getWeightFromPythonAPI(const std::string &jsonPoints, std::string &answer) {

    const std::string host = "127.0.0.1";
    const std::string target = "/api";

    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);

    boost::asio::ip::tcp::socket socket(ioc);

    //  установка соединения
    boost::asio::connect(socket, resolver.resolve(host, "5000"));

    //  создаем запрос
    http::request<http::string_body> req(http::verb::post, target, 11);

    //  устанавливаем поля http заголовка
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::accept_charset, "utf-8");
    req.content_length(jsonPoints.size());
    req.keep_alive(req.keep_alive());
    req.body() = jsonPoints;
    req.prepare_payload();

    //  отправляем запрос
    http::write(socket, req);

    //  читаем запрос
    std::stringstream answerStream;
    {
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> response;
        http::read(socket, buffer, response);
        answerStream << response;
    }

    //  закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    //  стираем заголовки из ответа
    answer = answerStream.str();
    size_t startJson = answer.find('{');
    answer.erase(0, startJson);
}

void Worker::setJsonAnswerInClass(const std::string &answer, const size_t pointCount){

    boost::property_tree::ptree answerTree;
    std::istringstream is(answer);
    boost::property_tree::json_parser::read_json(is, answerTree);

    answerTree = answerTree.get_child("weights");

    std::string key;
    for (size_t i = 0 ; i < pointCount ; i++)
        for (size_t j = 0 ; j < pointCount ; j++) {
            if (i == j)
                continue;
            key = std::to_string(i) + "->" + std::to_string(j);
            size_t distance = answerTree.get_child(key).get_value<size_t>() ;
            edges.push_back(std::make_pair(i, j));
            weightArr.push_back(distance);
        }
}


void Worker::GetRoute(const std::vector<Point> &points, const std::vector<std::vector<int>> &ribs,
                      std::list<Point> &res){

}