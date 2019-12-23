//
// Created by ser on 21.12.2019.
//

#include "server.hpp"
#include "queue.hpp"
//#include "worker.hpp"
#include "data.hpp"

int main(int* argv, char **argc) {
    GQueue<DataIn> QIn((DataIn(Filters(), 0, 0, Point(), Point(), -1)));
    GQueue<DataOut> QOut((DataOut(Points(), 0, -1)));

    DataOut out;

    out.RoutePoints.emplace_back(std::make_pair(12.1133, 23.343));
    out.RoutePoints.emplace_back(std::make_pair(45.342, 64.34));
    out.MaxTime = 4 * MIN_IN_HOUR + 23;
    out.UserID = 121;

    QOut.push(out);


    Server s(QIn, QOut, 6666);
    s.StartEchoServer();
    return 0;
}
