/*
 * Copyright 2019 <Copyright Owner>
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "server.hpp"
#include "queue.hpp"
#include "data.hpp"
#include "structures.pb.h"

class ClientPub{
 private:
    Client *c;
    boost::asio::io_service service;

 public:
    ClientPub() {
        c = new Client(service);
    };
    ~ClientPub() {
        delete(c);
    };
    
    In Unmarshal(const std::string& buffer) {
        return c->unmarshal(buffer);
    }
    std::string Marshal(const Out& out){
        return c->marshal(out);
    }
};

class ClientTest : public ::testing::Test {
 public:
    ClientTest() {};
    ~ClientTest() {};
 protected:
    void SetUp() override {
        in.Filters.push_back("Пушкин");
        in.Filters.push_back("Памятник");
        in.Filters.push_back("Музей");
        in.MaxDots = 5;
        in.StartPoint.first = 11.11; 
        in.StartPoint.second = 55.55;
        in.TimeLimit = 5*SEC_IN_HOUR;
        in.UserID = 121;
        
        Data::In protoIn;
        for (auto &&i : in.Filters) {
            protoIn.add_filters(i.c_str());
        }
        protoIn.set_timelimit(in.TimeLimit);
        protoIn.set_maxdots(in.MaxDots);
        Data::Point sp;
        sp.set_x(in.StartPoint.first);
        sp.set_y(in.StartPoint.second);
        protoIn.set_allocated_startpoint(&sp);

        out.RoutePoints.push_back(std::make_pair(12.1133, 23.343));
        out.RoutePoints.push_back(std::make_pair(45.342, 64.34));
        out.RoutePoints.push_back(std::make_pair(75.3434, 10.98));
        out.RoutePoints.push_back(std::make_pair(74.049, 23.343));
        out.RoutePoints.push_back(std::make_pair(34.566, 54.343));
        out.MaxTime = 4*SEC_IN_HOUR + 23*SEC_IN_MIN;
        out.UserID = 121;

        Data::Out protoOut;
        for (auto &&i : out.RoutePoints) {
            auto sp = protoOut.add_routepoints();
            sp->set_x(in.StartPoint.first);
            sp->set_y(in.StartPoint.second);
        }
        protoOut.set_maxtime(out.MaxTime);


        out_bytes = std::move(protoOut.SerializeAsString());
        in_bytes = std::move(protoIn.SerializeAsString());
    }
    

    In in;
    Out out;

    std::string in_bytes;
    std::string out_bytes;
};

TEST_F(ClientTest, MarshalTest) {
    ClientPub *c = new ClientPub();
    std::string marshaled(c->Marshal(out));
    ASSERT_EQ(marshaled, out_bytes);
    delete(c);
}

TEST_F(ClientTest, UnmarshalTest) {
    ClientPub c;
    In in_test = c.Unmarshal(in_bytes);
    ASSERT_EQ(in_test.Filters, in.Filters);
    ASSERT_EQ(in_test.MaxDots, in.MaxDots);
    ASSERT_EQ(in_test.StartPoint, in.StartPoint);
    ASSERT_EQ(in_test.TimeLimit, in.TimeLimit);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
