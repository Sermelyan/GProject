/*
 * Copyright 2019 <Copyright Owner>
 */

#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "data.hpp"
#include "queue.hpp"
#include "structures.pb.h"

#ifndef INCLUDE_SERVER_HPP_
#define INCLUDE_SERVER_HPP_

class ClientPub;

class Client {
 public:
    int user_id;

    explicit Client(const boost::asio::io_service &io) : sock(io) {}
    // Client() {};
    ~Client() {}
    void Read();
    void Write(const Out &out);
    friend class ClientPub;

 private:
    boost::asio::ip::tcp::socket sock;
    char buffer[1024];

    void handleRead(const boost::system::error_code &e);
    void handleWrite(const boost::system::error_code &e);

    In unmarshal(const std::string &buffer) {}
    std::string marshal(const Out &out) {}

    void sendToQueue(const In &data);
};

class Server {
 public:
    Server(Queue<In> *in, Queue<Out> *out, unsigned Port);
    ~Server();

    void Kill();
    void StartServer();

 private:
    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<Client>> waitingClients;
    boost::asio::io_service service;
    boost::asio::ip::tcp::acceptor acceptor;
    Queue<In> &in;
    Queue<Out> &out;

    void startAccept();
    void onAccept();
    void startSend();
    void onSend();

    Out GetFromQueue();
};

#endif  //  INCLUDE_SERVER_HPP_
