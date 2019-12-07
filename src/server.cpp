/*
 * Copyright 2019 <Copyright Owner>
 */

#include "server.hpp"
#include <boost/thread.hpp>
#include "data.hpp"

Server::Server(GQueue<DataIn> const *_in, GQueue<DataOut> const *_out,
               unsigned Port)
    : in(_in),
      out(_out),
      alive(true),
      port(Port),
      acceptor(service, boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(), Port)) {}

Server::~Server() = default;

void Server::Kill() { alive = false; }

void Server::StartServer() {
    startAccept();
    for (int i = 0; i < 4; ++i)
        threads.emplace_back(
            boost::bind(&boost::asio::io_service::run, &service));
    for (auto &thread : threads) thread.join();
}

void Server::StartEchoServer() {
    threads.emplace_back(std::bind(&Server::sillyServer, this));
    for (auto &thread : threads) {
        thread.join();
    }
}

void Server::startAccept() {
    std::shared_ptr<Client> c(new Client(service, in));
    acceptor.async_accept(c->Sock(),
                          std::bind(&Server::onAccept, this, c,
                                    boost::asio::placeholders::error));
}

void Server::onAccept(std::shared_ptr<Client> c, const error_code &e) {
    if (e) return;
    c->Read();
    startAccept();
}

DataOut Server::GetFromQueue() {
    while (alive) {
        auto data = out->popIfNotEmpty();
        if (data.UserID == -1) {
            boost::this_thread::sleep(boost::posix_time::millisec(100));
        } else {
            std::unique_ptr<Client> c;
            for (auto waitingClient : waitingClients) {
                c = waitingClient;
            }
            c->Write(data);
        }
    }
}

void Server::sillyServer() {
    while (alive) {
        boost::asio::ip::tcp::socket sock(service);
        acceptor.accept(sock);
        std::cout << "Server: Aviable bytes: " << sock.available() << std::endl;
        char buff[1024];
        auto bytes = sock.receive(boost::asio::buffer(buff));
        std::cout << "Server: Received bytes: " << bytes << std::endl;
        sock.write_some(boost::asio::buffer(buff, bytes));
        sock.close();
    }
}

// Client

Client::Client(boost::asio::io_service &io, GQueue<DataIn> const *_in)
    : sock(io), in(_in) {}

Client::~Client() = default;

boost::asio::ip::tcp::socket &Client::Sock() { return sock; }

void Client::Read() {
    sock.async_receive(boost::asio::buffer(buffer),
                       boost::bind(&Client::handleRead, this, buffer,
                                   boost::asio::placeholders::error));
}

void Client::Write(const DataOut &out) {}

void Client::handleRead(std::string data, const boost::system::error_code &e) {
    if (e) return;
    auto unmarshaled = unmarshal(data);
    in->push(*unmarshaled);
}

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
