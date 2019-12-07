/*
 * Copyright 2019 <Copyright Owner>
 */

#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "data.hpp"
#include "queue.hpp"
#include "structures.pb.h"

#ifndef INCLUDE_SERVER_HPP_
#define INCLUDE_SERVER_HPP_

class ClientAdapter;

class Client {
   public:
    int user_id;

    explicit Client(boost::asio::io_service &io, GQueue<DataIn> const *in);
    ~Client();
    void Read();
    void Write(const DataOut &out);
    boost::asio::ip::tcp::socket &Sock();

   private:
    friend class ClientAdapter;
    boost::asio::ip::tcp::socket sock;
    GQueue<DataIn> &in;
    char buffer[1024];

    void handleRead(std::string data, const boost::system::error_code &e);
    void handleWrite(const boost::system::error_code &e);

    static std::unique_ptr<DataIn> unmarshal(const std::string &buffer);
    static std::string marshal(const DataOut &out);
};

class Server {
   public:
    Server(GQueue<DataIn> const *in, GQueue<DataOut> const *out, unsigned Port);
    ~Server();

    void Kill();
    void StartServer();
    void StartEchoServer();

   private:
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<Client>> waitingClients;
    boost::asio::io_service service;
    boost::asio::ip::tcp::acceptor acceptor;
    GQueue<DataIn> &in;
    GQueue<DataOut> &out;
    bool alive;
    unsigned port;

    void startAccept();
    void onAccept(std::shared_ptr<Client> c, const error_code &e);
    void onSend(std::shared_ptr<Client> c, const DataOut &out,
                const error_code &e);

    void sillyServer();

    DataOut GetFromQueue();
};

#endif  //  INCLUDE_SERVER_HPP_
