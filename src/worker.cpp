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

//void Worker::GetRibsFromAPI(const std::vector<Point> &points){
//    for ( int i = 0; i < points.size(); ++i ) {
//        for(int j = 0; j < points.size() - 1; ++j ) {
//            weightArr.push_back(2);
//            edges.push_back(std::make_pair(i, j));
//        }
//    }
//}

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


// FROM WORKER API

void Worker::GetRibsFromAPI(const std::vector<Point> &points ) {

    const size_t pointCount = points.size();
    if (pointCount < MIN_POINT_COUNT || pointCount > MAX_POINT_COUNT)
        return;

    const std::string jsonForSending = createJsonForSending(points);

    std::string answer;
    getWeightFromPythonAPI(jsonForSending, answer);

    setJsonAnswerInClass(answer, pointCount);

}


std::string Worker::createJsonForSending(const std::vector<Point> &points) {

    boost::property_tree::ptree rootJsonTree;
    boost::property_tree::ptree coordinates;
    boost::property_tree::ptree coordinate;

    for (size_t i = 0 ; i < points.size() ; i++) {
        coordinate.put("X", points.at(i).first);
        coordinate.put("Y", points.at(i).second);
        // id координаты = её место в принимающем векторе
        std::string id = std::to_string(i);
        coordinates.add_child( id, coordinate);
    }

    rootJsonTree.add_child("coordinates", coordinates);
    std::stringstream jsonStream;
    boost::property_tree::write_json(jsonStream, rootJsonTree);

    return jsonStream.str();
}


void Worker::getWeightFromPythonAPI(const std::string &jsonPoints, std::string &answer) {

    const std::string host = "127.0.0.1";
    const std::string target = "/api";

    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);

    boost::asio::ip::tcp::socket socket(ioc);

    //  установка соединения
    boost::asio::connect(socket, resolver.resolve(host, "5000"));

    //  создаем запрос
    http::request<http::string_body> req(http::verb::post, target, 11);

    //  устанавливаем поля http заголовка
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.set(http::field::accept_charset, "utf-8");
    req.content_length(jsonPoints.size());
    req.keep_alive(req.keep_alive());
    req.body() = jsonPoints;
    req.prepare_payload();

    //  отправляем запрос
    http::write(socket, req);

    //  читаем запрос
    std::stringstream answerStream;
    {
        boost::beast::flat_buffer buffer;
        http::response<http::dynamic_body> response;
        http::read(socket, buffer, response);
        answerStream << response;
    }

    //  закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    //  стираем заголовки из ответа
    answer = answerStream.str();
    size_t startJson = answer.find('{');
    answer.erase(0, startJson);
}

void Worker::setJsonAnswerInClass(const std::string &json, const size_t &pointCount){

    boost::property_tree::ptree answerTree;
    std::istringstream is(json);
    boost::property_tree::json_parser::read_json(is, answerTree);

    answerTree = answerTree.get_child("weights");

    std::string key;
    for (size_t i = 0 ; i < pointCount ; i++)
        for (size_t j = 0 ; j < pointCount ; j++) {
            if (i == j)
                continue;
            key = std::to_string(i) + "->" + std::to_string(j);
            size_t distance = answerTree.get_child(key).get_value<size_t>() ;
            edges.push_back(std::make_pair(i, j));
            weightArr.push_back(distance);
        }
}

long int Worker::getWeightIndex(const size_t &pointsCount, const size_t &from, const size_t &to){

    if (from > pointsCount - 1 || to > pointsCount - 1 ||
        from == to)
        return -1; // return error

    if (from == 0)
        return to - 1;

    if (from < to)
       return ( (pointsCount -1) * from + to) - 1;

    return ( (pointsCount -1) * from + to);

}
// WORKER API END

