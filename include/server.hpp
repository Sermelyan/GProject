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

    explicit Client(boost::asio::io_service &io);
    Client() = default;
    ~Client();
    void Read();
    void Write(const DataOut &out);

   private:
    friend class ClientAdapter;
    boost::asio::ip::tcp::socket *sock;
    char buffer[1024];

    void handleRead(const boost::system::error_code &e);
    void handleWrite(const boost::system::error_code &e);

    static std::unique_ptr<DataIn> unmarshal(const std::string &buffer);
    static std::string marshal(const DataOut &out);

    void sendToQueue(const DataIn &data);
};

class Server {
   public:
    Server(Queue<DataIn> const *in, Queue<DataOut> const *out, unsigned Port);
    ~Server();

    void Kill();
    void StartServer();
    void StartEchoServer();

   private:
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<Client>> waitingClients;
    boost::asio::io_service service;
    boost::asio::ip::tcp::acceptor *acceptor;
    Queue<DataIn> const *in;
    Queue<DataOut> const *out;
    bool alive;
    unsigned port;

    void startAccept();
    void onAccept();
    void startSend();
    void onSend();

    void sillyServer();

    DataOut GetFromQueue();
};

#endif  //  INCLUDE_SERVER_HPP_
