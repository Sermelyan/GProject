/*
 * Copyright 2019 <Copyright Owner>
 */

#include "server.hpp"
#include <boost/thread.hpp>
#include "data.hpp"

Server::Server(Queue<DataIn> const *_in, Queue<DataOut> const *_out,
               unsigned Port)
    : in(_in),
      out(_out),
      alive(true),
      port(Port)
/*acceptor(service, boost::asio::ip::tcp::endpoint(
                      boost::asio::ip::tcp::v4(), Port))*/
{
    std::cout << "This constructor is working!" << std::endl;
}

Server::~Server() { std::cout << "This destructor is working!" << std::endl; }

void Server::Kill() { alive = false; }

void Server::StartServer() {}

void Server::StartEchoServer() {
    std::cout << "This start is working!" << std::endl;
    threads.emplace_back(std::bind(&Server::sillyServer, this));
    for (auto &thread : threads) {
        thread.join();
    }
}

void Server::startAccept() {}

void Server::onAccept() {}

void Server::startSend() {}

void Server::onSend() {}

DataOut Server::GetFromQueue() {}

void Server::sillyServer() {
    std::cout << "This tread is working!" << std::endl;
    acceptor = new boost::asio::ip::tcp::acceptor(
        service,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    while (alive) {
        std::cout << "This tread cycle is working!" << std::endl;
        boost::asio::ip::tcp::socket sock(service);
        acceptor->accept(sock);
        std::cout << "Server: Aviable bytes: " << sock.available() << std::endl;
        char buff[1024];
        auto bytes = sock.receive(boost::asio::buffer(buff));
        std::cout << "Server: Received bytes: " << bytes << " " << buff
                  << std::endl;
        std::cout << "Server: Got: " << buff << std::endl;
        sock.write_some(boost::asio::buffer(buff));
        sock.close();
    }
}

// Client

Client::Client(boost::asio::io_service &io) {}

Client::~Client() = default;

void Client::Read() {}

void Client::Write(const DataOut &out) {}

void Client::handleRead(const boost::system::error_code &e) {}

void Client::handleWrite(const boost::system::error_code &e) {}

std::unique_ptr<DataIn> Client::unmarshal(const std::string &buffer) {
    auto parsePoint = [](const Data::Point &p) -> Point {
        return std::make_pair(p.x(), p.y());
    };

    Data::In data;
    if (!data.ParseFromString(buffer)) return nullptr;
    Filters f;
    for (size_t i = 0; i < data.filters_size(); i++)
        f.push_back(data.filters(i));
    std::unique_ptr<DataIn> parsed(
        new DataIn(std::move(f), data.timelimit(), data.maxdots(),
                   std::move(parsePoint(data.startpoint())),
                   parsePoint(data.endpoint()), data.userid()));
    return parsed;
}

std::string Client::marshal(const DataOut &out) {
    Data::Out decoded;
    for (auto &&i : out.RoutePoints) {
        auto t = decoded.add_routepoints();
        t->set_x(i.first);
        t->set_y(i.second);
    }
    decoded.set_maxtime(out.MaxTime);
    decoded.set_userid(out.UserID);
    return decoded.SerializeAsString();
}

void Client::sendToQueue(const DataIn &data) {}
