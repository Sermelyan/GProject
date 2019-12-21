/*
 * Copyright 2019 <Copyright Owner>
 */

#include <algorithm>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>
#include <map>

#include "data.hpp"
#include "server.hpp"

Server::Server(GQueue<DataIn> &_in, GQueue<DataOut> &_out, unsigned Port)
    : in(_in), out(_out), alive(true), port(Port),
      acceptor(service, boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(), Port)) {}

Server::~Server() {
    BOOST_LOG_TRIVIAL(trace) << "Server(destructor): working.";
    for (auto &thread : threads) {
        thread.join();
    }
    BOOST_LOG_TRIVIAL(trace) << "Server(destructor): finished work.";
}

void Server::Kill() {
    const std::lock_guard<std::mutex> lock(liveMutex);
    BOOST_LOG_TRIVIAL(info) << "Server(Kill): killing at port: " << port;
    alive = false;
}

void Server::StartServer(unsigned serviceThr, unsigned queueThr) {
    BOOST_LOG_TRIVIAL(info)
        << "Server(StartServer): starting at port: " << port;
    StartAccept();
    for (int i = 0; i < serviceThr; ++i)
        threads.emplace_back(
            boost::bind(&boost::asio::io_service::run, &service));
    for (int i = 0; i < queueThr; ++i)
        threads.emplace_back(boost::bind(&Server::GetFromQueue, this));
}

void Server::StartAccept() {
    if (!isAlive())
        return;
    Client::ClientPtr c = Client::NewClient(service, *this);
    acceptor.async_accept(c->Sock(),
                          boost::bind(&Server::onAccept, this, c,
                                      boost::asio::placeholders::error));
}

void Server::onAccept(const Client::ClientPtr &c,
                      const boost::system::error_code &e) {
    if (e) {
        BOOST_LOG_TRIVIAL(error) << "Server(onAccept): error: " << e;
        return;
    }
    BOOST_LOG_TRIVIAL(debug)
        << "Server(onAccept): new client at port: " << port;
    c->Read();
    StartAccept();
}

void Server::GetFromQueue() {
    BOOST_LOG_TRIVIAL(debug) << "Server(GetFromQueue): started thread.";
    while (alive) {
        auto data = out.popIfNotEmpty();
        auto id = data.UserID;
        if (id > -1) {
            BOOST_LOG_TRIVIAL(trace)
                << "Server(GetFromQueue): poped data for user with id: " << id;
            waitingMutex.lock();
            auto cIter = std::find_if(
                waitingClients.begin(), waitingClients.end(),
                [id](const Client::ClientPtr &c) { return c->user_id == id; });
            if (cIter != waitingClients.end()) {
                auto c = cIter->get();
                c->Write(Client::Marshal(data));
            } else {
                BOOST_LOG_TRIVIAL(error)
                    << "Server(GetFromQueue): didn't find user with id: " << id;
            }
            waitingMutex.unlock();
            boost::this_thread::sleep(boost::posix_time::millisec(5));
        } else {
            boost::this_thread::sleep(boost::posix_time::millisec(1000));
        }
    }
}

void Server::SendToQueue(std::unique_ptr<DataIn> data) { in.push(*data); }

void Server::AddWaitingClient(const Client::ClientPtr &c) {
    const std::lock_guard<std::mutex> lock(waitingMutex);
    BOOST_LOG_TRIVIAL(trace)
        << "Server(AddWaitingClient): added client: " << c->user_id
        << "to waitingClients";
    waitingClients.insert(c);
}

void Server::RemoveWaitingClient(const Client::ClientPtr &c) {
    const std::lock_guard<std::mutex> lock(waitingMutex);
    BOOST_LOG_TRIVIAL(trace)
        << "Server(RemoveWaitingClient): removed client: " << c->user_id
        << "from waitingClients";
    waitingClients.erase(c);
}

bool Server::isAlive() {
    const std::lock_guard<std::mutex> lock(liveMutex);
    return alive;
}

void Server::StartEchoServer() {
    threads.emplace_back(std::bind(&Server::sillyServer, this));
}

void Server::sillyServer() {
    while (isAlive()) {
        boost::asio::ip::tcp::socket sock(service);
        acceptor.accept(sock);
        std::cout << "Server: Aviable bytes: " << sock.available() << std::endl;
        char buff[1024];
        auto bytes = sock.receive(boost::asio::buffer(buff));
        std::cout << "Server: Received bytes: " << bytes << std::endl;
        sock.write_some(boost::asio::buffer(buff, bytes));
        sock.close();
        boost::this_thread::sleep(boost::posix_time::millisec(1));
    }
}

// Client

Client::Client(boost::asio::io_service &io, Server &s)
    : user_id(-1), _socket(io), _server(s) {}

boost::asio::ip::tcp::socket &Client::Sock() { return _socket; }

Client::ClientPtr Client::NewClient(boost::asio::io_service &io, Server &s) {
    BOOST_LOG_TRIVIAL(trace) << "Client(NewClient): created new client.";
    return Client::ClientPtr(new Client(io, s));
}

void Client::Read() {
    BOOST_LOG_TRIVIAL(trace)
        << "Client(Read): started read. aviaelable:" << _socket.available();
    _socket.async_receive(
        boost::asio::buffer(_read_msg, _socket.available()),
        boost::bind(&Client::onRead, this, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    BOOST_LOG_TRIVIAL(trace) << "Client(Read): finished read.";
}

void Client::onRead(const boost::system::error_code &e, size_t bytes) {
    if (e) {
        BOOST_LOG_TRIVIAL(error) << "Client(onRead): error: " << e;
        return;
    }
    BOOST_LOG_TRIVIAL(trace) << "Client(onRead): recived: " << _read_msg;
    BOOST_LOG_TRIVIAL(trace) << "Client(onRead): red bytes: " << bytes;
    auto in = Unmarshal(_read_msg);
    if (in) {
        user_id = in->UserID;
        BOOST_LOG_TRIVIAL(debug)
            << "Client(onRead): got data from client with id: " << user_id;
        _server.SendToQueue(std::move(in));
        _server.AddWaitingClient(shared_from_this());
    } else {
        BOOST_LOG_TRIVIAL(error)
            << "Client(onRead): client didn't send any valid data.";
    }
    _server.StartAccept();
}

void Client::Write(const std::string &msg) {
    _write_msg = msg;
    _socket.async_write_some(
        boost::asio::buffer(_write_msg, msg.size()),
        boost::bind(&Client::onWrite, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Client::onWrite(const boost::system::error_code &e, size_t bytes) {
    if (e) {
        BOOST_LOG_TRIVIAL(error) << "Client(onWrite): error: " << e;
        return;
    }
    BOOST_LOG_TRIVIAL(debug)
        << "Client(onWrite): send data to client with id: " << user_id;
    _server.RemoveWaitingClient(shared_from_this());
    _server.StartAccept();
}

void Client::parseHTTP(const char *msg, const char *msg_end, std::map<std::string, std::string> &httpRequest) {
    const char *head = msg;
    const char *tail = msg;

    // Find request type
    while (tail != msg_end && *tail != ' ') ++tail;
    httpRequest["Type"] = std::string(head, tail);

    // Find path
    while (tail != msg_end && *tail == ' ') ++tail;
    head = tail;
    while (tail != msg_end && *tail != ' ') ++tail;
    httpRequest["Path"] = std::string(head, tail);

    // Find HTTP version
    while (tail != msg_end && *tail == ' ') ++tail;
    head = tail;
    while (tail != msg_end && *tail != '\n') ++tail;
    httpRequest["Version"] = std::string(head, tail);
    if (tail != msg_end) ++tail;
    head = tail;
    while (head != msg_end && *head != '\n') {
        while (tail != msg_end && *tail != '\n') ++tail;
        const char *colon = static_cast<const char *>(memchr(head, ':', tail - head));
        if (colon == NULL) {
            // TODO: malformed headers, what should happen?
            break;
        }
        const char *value = colon+1;
        while (value != tail && *value == ' ') ++value;
        httpRequest[ std::string(head, colon) ] = std::string(value, tail);
        head = tail+1;
        tail++;
    }
}

// std::unique_ptr<DataIn> Client::Unmarshal(const std::string &buffer) {
//    auto parsePoint = [](const Data::Point &p) -> Point {
//        return std::make_pair(p.x(), p.y());
//    };
//
//    Data::In data;
//    if (!data.ParseFromString(buffer))
//        return nullptr;
//    Filters f;
//    for (size_t i = 0; i < data.filters_size(); i++)
//        f.push_back(data.filters(i));
//    std::unique_ptr<DataIn> parsed(
//        new DataIn(std::move(f), data.timelimit(), data.maxdots(),
//                   std::move(parsePoint(data.startpoint())),
//                   parsePoint(data.endpoint()), data.userid()));
//    return parsed;
//}

std::unique_ptr<DataIn> Client::Unmarshal(const char *msg) {
    std::string buffer(msg);
    if (buffer.empty()) {
        BOOST_LOG_TRIVIAL(error)
            << "Client(unmarshal): invalid string (zero size).";
        return nullptr;
    }
    std::map<std::string, std::string> httpReq;
    parseHTTP(msg, msg + buffer.size(), httpReq);
    for (const auto &item : httpReq) {
        std::cout << item.first << " " << item.second << std::endl;
    }
    BOOST_LOG_TRIVIAL(trace)
        << "Client(unmarshal): startet unmarshal string: " << buffer;
    try {
        std::stringstream ss(buffer);
        boost::property_tree::ptree dataTree;
        boost::property_tree::json_parser::read_json(ss, dataTree);
        Filters filters;
        for (const auto &filter : dataTree.get_child("filterList")) {
            if (!filter.first.empty()) {
                BOOST_LOG_TRIVIAL(error)
                    << "Client(unmarshal): invalid structure of filters array.";
                break;
            }
            filters.push_back(filter.second.get_value<std::string>(""));
        }
        std::unique_ptr<DataIn> parsed(new DataIn(
            filters, dataTree.get_child("timeLimit").get_value<unsigned>(0),
            dataTree.get_child("maxDots").get_value<unsigned>(0),
            Point(dataTree.get_child("startPoint.x").get_value<double>(),
                  dataTree.get_child("startPoint.y").get_value<double>()),
            Point(dataTree.get_child("endPoint.x").get_value<double>(0.0),
                  dataTree.get_child("endPoint.y").get_value<double>(0.0)),
            dataTree.get_child("userID").get_value<int>(-1)));
        return parsed;
    } catch (std::exception const &e) {
        BOOST_LOG_TRIVIAL(error) << "Client(unmarshal): " << e.what();
    }
    return nullptr;
}

// std::string Client::Marshal(const DataOut &out) {
//    Data::Out decoded;
//    for (auto &&i : out.RoutePoints) {
//        auto t = new Data::Point;
//        t->set_x(i.first);
//        t->set_y(i.second);
//        decoded.set_allocated_routepoints(t);
//    }
//    decoded.set_maxtime(out.MaxTime);
//    decoded.set_userid(out.UserID);
//    return decoded.SerializeAsString();
//}

std::string Client::Marshal(const DataOut &out) {
    if (out.UserID < 0) {
        BOOST_LOG_TRIVIAL(error) << "Client(marshal): invalid user id.";
        return "";
    }
    boost::property_tree::ptree rootJsonTree;
    boost::property_tree::ptree points;
    rootJsonTree.add("userID", out.UserID);
    rootJsonTree.add("maxTime", out.MaxTime);
    for (const auto &routePoint : out.RoutePoints) {
        boost::property_tree::ptree point;
        point.put("x", routePoint.first);
        point.put("y", routePoint.first);
        points.push_back(std::make_pair("", point));
    }
    rootJsonTree.add_child("points", points);
    std::stringstream mStream;
    boost::property_tree::write_json(mStream, rootJsonTree);
    return mStream.str();
}