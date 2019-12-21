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
    DataIn a = In.popIfNotEmpty();
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

void Worker::GetRoutefromAlgorithm(const std::vector<Algorithm::edge>  edge, const std::vector<size_t> weight,
                      std::pair<std::vector<Algorithm::dotId >, size_t> &res, size_t num_dots, DataIn value){
    Algorithm way(edge, weight); //  из апи 2 массива
    res = way.getRoute(0, num_dots, value.TimeLimit, value.MaxDots);
}

void Worker::FinalPoints(std::vector<Point> &points, const std::pair<std::vector<Algorithm::dotId >, size_t> &res){
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
            std::pair<std::vector<Algorithm::dotId >, size_t> Res;
            GetRoutefromAlgorithm(edges, weightArr, Res, PointsResFromDB.size(), value); // !!!!!!!!!!!
            FinalPoints(PointsResFromDB, Res);
            DataOut OutValue;
            OutValue.UserID = value.UserID;
            OutValue.MaxTime = Res.second;
            OutValue.RoutePoints = PointsResFromDB;
            SendToQueueOut(OutValue);
        }
    }
}

void Worker::GetRest(int timeToSleep) {

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


