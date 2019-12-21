/*
 * Copyright 2019 <Copyright Owner>
 */

#include <memory>
#include <set>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "data.hpp"
#include "queue.hpp"

#ifndef INCLUDE_SERVER_HPP_
#define INCLUDE_SERVER_HPP_

class ClientAdapter;
class Server;

class Client : std::enable_shared_from_this<Client> {
  public:
    int user_id;
    typedef std::shared_ptr<Client> ClientPtr;

    boost::asio::ip::tcp::socket &Sock();
    static ClientPtr NewClient(boost::asio::io_service &io, Server &s);

    void Read();
    void Write(const std::string &msg);

    static std::unique_ptr<DataIn> Unmarshal(const char *msg);
    static std::string Marshal(const DataOut &out);

  private:
    explicit Client(boost::asio::io_service &io, Server &s);

    boost::asio::ip::tcp::socket _socket;
    Server &_server;
    //    std::string _read_msg;
    //    std::string _write_msg;

    char _read_msg[4096];
    std::string _write_msg;

    void onRead(const boost::system::error_code &e, size_t bytes);
    void onWrite(const boost::system::error_code &e, size_t bytes);
    static void parseHTTP(const char *msg, const char *msg_end, std::map<std::string, std::string> &httpRequest);
};

class Server {
  public:
    Server(GQueue<DataIn> &in, GQueue<DataOut> &out, unsigned Port);
    ~Server();

    void Kill();
    void SendToQueue(std::unique_ptr<DataIn> data);
    void StartAccept();
    void StartEchoServer();
    void StartServer(unsigned serviceThr = 4, unsigned queueThr = 2);

    void AddWaitingClient(const Client::ClientPtr &c);
    void RemoveWaitingClient(const Client::ClientPtr &c);

  private:
    boost::asio::io_service service;
    GQueue<DataIn> &in;
    GQueue<DataOut> &out;
    bool alive;
    unsigned port;

    boost::asio::ip::tcp::acceptor acceptor;
    std::vector<std::thread> threads;
    std::set<Client::ClientPtr> waitingClients;
    std::mutex liveMutex;
    std::mutex waitingMutex;

    void onAccept(const Client::ClientPtr &c,
                  const boost::system::error_code &e);
    bool isAlive();
    void GetFromQueue();

    void sillyServer();
};

#endif //  INCLUDE_SERVER_HPP_
