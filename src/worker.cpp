#include "worker.hpp"
#include "queue.hpp"


Worker::Worker(GQueue<DataIn> &in, GQueue<DataOut> &out, const char *DBName):In(in), Out(out), DB(DBName), Stop(false), WProces(std::bind(&Worker::WorkerProcess, this)) {
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
    // разобрать дата ин и отправить запрос
//    SELECT * FROM table_name
//    WHERE column_name IN (value1, value2, ...);
    std::string SqlRequest = "SELECT * FROM test WHERE Filters IN (";
    for ( int i = 0; i < value.FilterList.size(); ++i ) {
        if ( i != 0 ) {
            SqlRequest += ", " + value.FilterList[i];
        } else {
            SqlRequest += value.FilterList[i];
        }
    }
    SqlRequest += ");";
    DB.Select(SqlRequest, points);
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
                      std::pair<std::vector<int>, size_t> &res, size_t num_dots, DataIn value){
    //вызов алгоритма
//    algorithm way(edge, weight); //  из апи 2 массива
//    res = way.getRoute(value.StartPoint, num_dots, value.TimeLimit, value.MaxDots);
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
            GetRoute(edges, weightArr, Res, PointsResFromDB.size(), value);
            FinalPoints(PointsResFromDB, Res);
//            DataOut OutValue(PointsResFromDB,Res.second,value.UserID);
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
//    rc = sqlite3_open(filename, &DB);
}

void Sqlite::Select(std::string sql, std::vector<Point> res) {
    std::pair<double, double> a;
    for ( int i = 0; i < 5; ++i ) {
        a.first = i;
        a.second = i;
        res.push_back(a);
    }

//    const char* data = "Callback function called";
//    if( rc ) {
//        fprintf(stderr, "Cant open database: %s\n", sqlite3_errmsg(DB));
////        return(0);
//    } else {
//        fprintf(stderr, "Opened \n");
//    }
//    rc = sqlite3_exec(DB, sql, callback, (void*)data, &zErrMsg);
//
//    if( rc != SQLITE_OK ) {
//        fprintf(stderr, "SQL error: %s \n", zErrMsg);
//        sqlite3_free(zErrMsg);
//    } else {
//        fprintf(stdout, "Successfully \n");
//    }
}

Sqlite::~Sqlite() {
//    sqlite3_close(DB);
}


