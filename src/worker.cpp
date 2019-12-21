#include "worker.hpp"
#include "queue.hpp"
#include "algorithm.hpp"
#include <iostream>


Worker::Worker(GQueue<DataIn> &in, GQueue<DataOut> &out, std::string DBName):In(in), Out(out), DB(DBName), Stop(false),
    WProces(std::bind(&Worker::WorkerProcess, this)) {
}

Worker::~Worker(){
    Stop = true;
    printf("End WorkerProc \n");
    WProces.join();
}

DataIn Worker::GetFromQueueIn(){
    DataIn a;
    return a;
}

void Worker::SendToQueueOut(const DataOut &value){
    Out.push(value);
}
void Worker::GetDotsFromDB(const DataIn &value, std::vector<Point> &points){
    DB.SelectTag(value.FilterList, points);
}

void Worker::GetRibsFromAPI(const std::vector<Point> &points){
    for ( int i = 0; i < points.size(); ++i ) {
        for(int j = 0; j < points.size() - 1; ++j ) {
            weightArr.push_back(2);
            edges.push_back(std::make_pair(i, j));
        }
    }
}

//void Worker::GetRoute(std::vector<Algorithm::dotId>  edges, std::vector<Algorithm::weight> weightArr,
//                               std::pair<std::vector<int>, size_t> &res, size_t num_dots, DataIn value);{
//    //вызов алгоритма
//    Algorithm way(edge, weight); //  из апи 2 массива
//    std::pair<std::vector<size_t>, size_t> R;
//    std::pair<size_t, size_t> toALg;
//    R = way.getRoute(0, num_dots, value.TimeLimit, value.MaxDots);
//    for ( auto i : R.first ) {
//        res.first.push_back(i);
//    }
//    res.second = R.second;
//}

void Worker::FinalPoints(std::vector<Point> &points, const std::pair<std::vector<int>, size_t> &res){
    std::vector<Point> buf;
    for( int i = 0;  i < res.first.size(); ++i ) {
        buf.push_back(points[res.first[i]]);
    }
    points.clear();
    points = buf;
}

void Worker::WorkerProcess(){
    int counter = 1;
    while ( !Stop ) {
        DataIn value = GetFromQueueIn();
        if ( value.UserID == -1) {
            sleep(200);
        } else {
            std::vector<Point> PointsResFromDB;
            GetDotsFromDB(value, PointsResFromDB);
            std::vector<std::vector<double>> RibsResFromApi;
            GetRibsFromAPI(PointsResFromDB);
            std::pair<std::vector<int>, size_t> Res;
//            GetRoute(edges, weightArr, Res, PointsResFromDB.size(), value);
            Algorithm algo(edges,weightArr);
            size_t t = value.TimeLimit;
            Res = algo.getRoute(0, PointsResFromDB.size(),
                    t, value.MaxDots);
            FinalPoints(PointsResFromDB, Res);
            DataOut OutValue;
            OutValue.UserID = value.UserID;
            OutValue.MaxTime = Res.second;
            OutValue.RoutePoints = PointsResFromDB;
            SendToQueueOut(OutValue);
        }
    }
}

void Worker::Kill() {
    Stop = true;
}

//=================================================================================================================

Table::Table(std::string filename) {
    file.open(filename);
    if (!file.is_open()) // если файл не открыт
        printf("Файл не может быть открыт!\n");
    else {
        std::string delim("#");
        std::string delitel(" ");
        std::string TagsBuf;
        Line buf;
        std::string buffer;
        while(getline(file,buffer)) {
            size_t prev = 0;
            size_t next;
            size_t delta = delim.length();
            size_t delta2 = delitel.length();
            size_t prev2 = 0;
            size_t next2 = buffer.find(delitel, prev2);
            buf.id = atoi(buffer.substr(prev2, next2-prev2).c_str()); // atoi( str.c_str() );
            prev2 = next2 + delta2;
            next2 = buffer.find(delitel, prev2);
            buf.x = atoi(buffer.substr(prev2, next2-prev2).c_str());
            prev2 = next2 + delta2;
            next2 = buffer.find(delitel, prev2);
            buf.y = atoi(buffer.substr(prev2, next2-prev2).c_str());
            prev2 = next2 + delta2;
            next2 = buffer.find(delitel, prev2);
            buf.Name = (buffer.substr(prev2, next2-prev2));
            prev2 = next2 + delta2;
            next2 = buffer.find(delitel, prev2);
            TagsBuf = (buffer.substr(prev2, next2-prev2));
            while( ( next = TagsBuf.find( delim, prev ) ) != std::string::npos ){
//                printf("lag");
                buf.Tags.push_back(TagsBuf.substr(prev, next-prev));
                prev = next + delta;
            }
            buf.Tags.push_back(TagsBuf.substr(prev));
            table.push_back(buf);
            buf.Tags.clear();
        }
    }
}

void Table::SelectTag(std::vector<std::string> values, std::vector<Point>& res) {
    bool flag = false;
    for ( auto lineElem : table ) {
        for (int i = 0; i < values.size() && !flag; ++i) {
            for (int j = 0; j < lineElem.Tags.size() && !flag; ++j) {
                if ( values[i] == lineElem.Tags[j] ) {
                    res.push_back(std::make_pair(lineElem.x, lineElem.y));
                    flag = true;
                }
            }
        }
        flag = false;
    }
}
void Table::SelectAll(std::vector<Point>& res){
    for ( auto i : table) {
        res.emplace_back(i.x, i.y);
    }
}

Table::~Table() {
    file.close();
}


