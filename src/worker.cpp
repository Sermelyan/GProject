#include "worker.hpp"
#include "queu.hpp"

Worker::Worker(Queue &in, Queue &out, const char *DBName):In(in), Out(out), DB(DBName), Stop(false), WProces(std::bind(&Worker::WorkerProcess, this)) { //TODO добавить апи
}

Worker::~Worker(){
    printf("End WorkerProc \n");
    WProces.join();
}

DataIn Worker::GetFromQueueIn(){
    DataIn a;
    return a;
}

void Worker::SendToQueueOut(const DataOut &value){
//    Out.push(value)
}
void Worker::GetDotsFromDB(const DataIn &value, std::vector<Point> &points){
    // разобрать дата ин и отправить скл запрос
//    Data_in.
}

void Worker::GetRibsFromAPI(const std::vector<Point> &points){
    for ( int i = 0; i < points.size(); ++i ) {
        for(int j = 0; j < points.size() - 1; ++j ) {
            weightArr.push_back(2);
            edges.push_back(std::make_pair(i, j));
        }
    }
}

void Worker::GetRoute(const std::vector<std::pair<size_t,size_t>>  edge, const std::vector<size_t> weight,
                      std::pair<std::vector<int>, size_t> &res){
    //вызов алгоритма
//    algorithm way(edge, weight); //  из апи 2 массива
//    way.getRoute();
}

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
    printf("Start WorkerProc \n");
    while ( !Stop ) {
        DataIn value = GetFromQueueIn();
        DataIn Incorect;
        if ( value == Incorect ) {
            sleep(200);
        } else {
            std::vector<Point> PointsResFromDB;
            GetDotsFromDB(value, PointsResFromDB);
            std::vector<std::vector<double>> RibsResFromApi;
            GetRibsFromAPI(PointsResFromDB);
            std::pair<std::vector<int>, size_t> Res;
            GetRoute(edges, weightArr, Res);
            FinalPoints(PointsResFromDB, Res);
            DataOut OutValue(PointsResFromDB,Res.second,value.UserID);
            SendToQueueOut(OutValue);
        }
    }
}

void Worker::Kill() {
    Stop = true;
}

//=================================================================================================================

static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

Sqlite::Sqlite(const char * filename) {
    zErrMsg = 0;
    rc = sqlite3_open(filename, &DB);
}

void Sqlite::Select(const char *sql, std::vector<Point> res) {
    const char* data = "Callback function called";
    if( rc ) {
        fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(DB));
//        return(0);
    } else {
        fprintf(stderr, "Opened \n");
    }
    rc = sqlite3_exec(DB, sql, callback, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s \n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Successfully \n");
    }
}

Sqlite::~Sqlite() {
    sqlite3_close(DB);
}


