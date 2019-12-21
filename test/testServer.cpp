/*
 * Copyright 2019 <Sergey Melikyan>
 */

#include <boost/thread.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "queue.hpp"
#include "server.hpp"

class ClientTest : public ::testing::Test {
  protected:
    boost::asio::io_service service;
    void SetUp() override {
        in.FilterList.push_back("Пушкин");
        in.FilterList.push_back("Памятник");
        in.FilterList.push_back("Музей");
        in.MaxDots = 5;
        in.StartPoint.first = 11.11;
        in.StartPoint.second = 55.55;
        in.EndPoint.first = 11.13;
        in.EndPoint.second = 55.51;
        in.TimeLimit = 5 * MIN_IN_HOUR;
        in.UserID = 121;

        out.RoutePoints.emplace_back(std::make_pair(12.1133, 23.343));
        out.RoutePoints.emplace_back(std::make_pair(45.342, 64.34));
        out.MaxTime = 4 * MIN_IN_HOUR + 23;
        out.UserID = 121;

        out_json = "{\n    \"userID\": \"121\",\n    \"maxTime\": \"263\",\n   "
                   " \"points\": [\n        {\n            \"x\": "
                   "\"12.113300000000001\",\n            \"y\": "
                   "\"12.113300000000001\"\n        },\n        {\n            "
                   "\"x\": \"45.341999999999999\",\n            \"y\": "
                   "\"45.341999999999999\"\n        }\n    ]\n}\n";
        in_json = "{\"userID\": 121, \"filterList\": [\"Пушкин\", "
                  "\"Памятник\", \"Музей\"], \"maxDots\": 5, \"timeLimit\": "
                  "300, \"startPoint\": {\"x\": 11.11, \"y\": 55.55}, "
                  "\"endPoint\": {\"x\": 11.13, \"y\": 55.51}}";
    }

    DataIn in;
    DataOut out;

    std::string in_json;
    std::string out_json;
};

TEST_F(ClientTest, MarshalTest) {
    std::string marshaled(Client::Marshal(out));
    ASSERT_EQ(marshaled, out_json);
}

TEST_F(ClientTest, UnmarshalTest) {
    auto in_test = Client::Unmarshal(in_json.c_str());
    ASSERT_EQ(in_test->FilterList, in.FilterList);
    ASSERT_EQ(in_test->MaxDots, in.MaxDots);
    ASSERT_EQ(in_test->StartPoint, in.StartPoint);
    ASSERT_EQ(in_test->EndPoint, in.EndPoint);
    ASSERT_EQ(in_test->TimeLimit, in.TimeLimit);
}

class SyncClient {
  public:
    SyncClient() : sock(service) {
        sock.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), 6666));
    }

    ~SyncClient() {
        if (sock.is_open())
            sock.close();
    }

    void Write(const std::string &msg) {
        auto bytes = sock.send(boost::asio::buffer(msg));
        std::cout << "Client: Send bytes: " << bytes << std::endl;
    }

    std::string Read() {
        char buf[1024];
        auto bytes = sock.receive(boost::asio::buffer(buf));
        std::cout << "Client: Got bytes: " << bytes << std::endl;
        return std::string(buf, bytes);
    }

  private:
    boost::asio::io_service service;
    boost::asio::ip::tcp::socket sock;
};

TEST_F(ClientTest, EchoTest1) {
    GQueue<DataIn> QIn((DataIn()));
    GQueue<DataOut> QOut((DataOut()));
    Server s(QIn, QOut, 6666);
    std::thread server_thr(std::bind(&Server::StartEchoServer, &s));
    boost::this_thread::sleep(boost::posix_time::millisec(100));
    std::string str("Test echo string!");
    SyncClient c;
    c.Write(str);
    std::string ans = c.Read();
    s.Kill();
    server_thr.join();
    ASSERT_EQ(str, ans);
}

TEST_F(ClientTest, EchoTest2) {
    GQueue<DataIn> QIn((DataIn()));
    GQueue<DataOut> QOut((DataOut()));
    Server s(QIn, QOut, 6666);
    std::thread server_thr(std::bind(&Server::StartEchoServer, &s));
    boost::this_thread::sleep(boost::posix_time::millisec(100));
    SyncClient c;
    c.Write(in_json);
    std::string ans(c.Read());
    s.Kill();
    server_thr.join();
    ASSERT_EQ(in_json, ans);
}

TEST(ServerTest, Test) {
    GQueue<DataIn> QIn((DataIn(Filters(), 0, 0, Point(), Point(), -1)));
    GQueue<DataOut> QOut((DataOut(Points(), 0, -1)));
    Server s(QIn, QOut, 6666);
    s.StartServer();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
