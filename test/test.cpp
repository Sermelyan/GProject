/*
 * Copyright 2019 <Copyright Owner>
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include <boost/thread.hpp>
#include "data.hpp"
#include "queue.hpp"
#include "server.hpp"
#include "structures.pb.h"

class ClientAdapter {
   private:
    Client c;

   public:
    ClientAdapter() = default;
    ~ClientAdapter() = default;

    std::unique_ptr<DataIn> Unmarshal(const std::string& buffer) {
        return c.unmarshal(buffer);
    }
    std::string Marshal(const DataOut& out) { return c.marshal(out); }
};

class ClientTest : public ::testing::Test {
   protected:
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

        Data::In protoIn;
        for (auto&& i : in.FilterList) {
            protoIn.add_filters(i.c_str());
        }
        protoIn.set_timelimit(in.TimeLimit);
        protoIn.set_maxdots(in.MaxDots);
        auto sp = new Data::Point;
        sp->set_x(in.StartPoint.first);
        sp->set_y(in.StartPoint.second);
        protoIn.set_allocated_startpoint(sp);
        auto ep = new Data::Point;
        ep->set_x(in.EndPoint.first);
        ep->set_y(in.EndPoint.second);
        protoIn.set_allocated_endpoint(ep);

        out.RoutePoints.emplace_back(std::make_pair(12.1133, 23.343));
        out.RoutePoints.emplace_back(std::make_pair(45.342, 64.34));
        out.RoutePoints.emplace_back(std::make_pair(75.3434, 10.98));
        out.RoutePoints.emplace_back(std::make_pair(74.049, 23.343));
        out.RoutePoints.emplace_back(std::make_pair(34.566, 54.343));
        out.MaxTime = 4 * MIN_IN_HOUR + 23;
        out.UserID = 121;

        Data::Out protoOut;
        for (auto&& i : out.RoutePoints) {
            auto sp = protoOut.add_routepoints();
            sp->set_x(i.first);
            sp->set_y(i.second);
        }
        protoOut.set_maxtime(out.MaxTime);
        protoOut.set_userid(out.UserID);

        out_bytes = protoOut.SerializeAsString();
        in_bytes = protoIn.SerializeAsString();
    }

    DataIn in;
    DataOut out;

    std::string in_bytes;
    std::string out_bytes;
};

TEST_F(ClientTest, MarshalTest) {
    ClientAdapter c;
    std::string marshaled(c.Marshal(out));
    ASSERT_EQ(marshaled, out_bytes);
}

TEST_F(ClientTest, UnmarshalTest) {
    ClientAdapter c;
    auto in_test = c.Unmarshal(in_bytes);
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
        std::cout << "Client constructor is working " << std::endl;
    }

    virtual ~SyncClient() { sock.close(); }

    void Write(const std::string& msg) {
        std::cout << "Client: Prepare Send: " << msg << std::endl;
        auto bytes = sock.send(boost::asio::buffer(msg));
        std::cout << "Client: Send bytes: " << bytes << std::endl;
        std::cout << "Client: Send: " << msg << std::endl;
    }

    std::string Read() {
        char buf[1024];
        sock.receive(boost::asio::buffer(buf));
        return buf;
    }

   private:
    boost::asio::io_service service;
    boost::asio::ip::tcp::socket sock;
};

class ServerTest : public ::testing::Test {
   protected:
    //    Server* s;
    //    void SetUp() override {
    //        s = new Server(nullptr, nullptr, 6666);
    //        s->StartServer();
    //    }
    //    void TearDown() override {
    //        s->Kill();
    //        delete(s);
    //    }
};

TEST_F(ServerTest, EchoTest) {
    Server s(nullptr, nullptr, 6666);
    std::thread server_thr(std::bind(&Server::StartEchoServer, &s));
    boost::this_thread::sleep(boost::posix_time::millisec(100));
    std::string str("Test echo string!");
    SyncClient c;
    c.Write(str);
    std::string ans = c.Read();
    s.Kill();
    server_thr.join();
    ASSERT_EQ(str, ans);
    std::cout << "Message get: " << ans << std::endl;
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
