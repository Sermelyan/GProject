#include "worker.hpp"
#include "queu.hpp"

//Worker::Worker()
//{
//
//}

Worker::Worker(Queue &in, Queue &out, Sqlite &DataBase):In(in), Out(out), DB(DataBase)
{
}

Worker::~Worker(){

}

Data_in Worker::GetFromQueueIn(){
    Data_in a;
    return a;
}

void  Worker::SendToQueueOut(const Data_out &value){

}
void Worker::GetDotsFromDB(const Data_in &value, std::vector<Point> &points){

}

void Worker::GetRibsFromAPI(const std::vector<Point> &points, std::vector<std::vector<double >> &ribs){

}

void Worker::GetRoute(const std::vector<Point> &points, const std::vector<std::vector<int>> &ribs,
                      std::list<Point> &res){

}

void Worker::WorkerProcess()
{

}

void Worker::Kill() {

}

Data_in::Data_in()
{

}

Data_in::Data_in(const std::vector<std::string> &f, const int id, const Limit l){
    for(const auto & i : f) {
        Filters.push_back(i);
    }
    User_id = id;
    for( int i = 0; i < 4; ++i ) {
        limits.Point[i] = l.Point[i];
    }
    limits.Time = l.Time;
}

Data_in::~Data_in()
{
    this->Filters.clear();
}

Limit Data_in::Get_Limit() const
{
    Limit a;
    return a;
}

void Data_in::Get_Filters(std::vector<std::string> &filtr) const
{

}

void Data_in::Set_Limit(const Limit value)
{

}

void Data_in::Set_Filters(const std::vector<std::string> &filtr)
{

}

Data_in Data_in::operator=(const Data_in &Reight)
{
    Data_in a;
    return a;
}

bool operator==(const Data_in left, const Data_in right){
    return true;
}

bool operator==(const Point left, const Point right){
//    if(left.X == right.X && left.Y == right.Y){
//        return true;
//    }
    return true;
}

bool operator==(const Data_out left, const Data_out right){
    return true;
}

Data_out::Data_out()
{

}

Data_out::Data_out(const std::vector<Point> &point, const int id, const int time)
{
    this->Points.clear();
//    for(int i = 0; i < point.size(); ++i){
//        Points.push_back(point[i].X);
//        Points[i].Y.push_back(point[i].Y);
//    }
    this->User_id = id;
    this->Time = time;
}

Data_out::~Data_out()
{
    this->Points.clear();
}

void Data_out::Get_Points(std::vector<std::pair<double, double >> point) const
{

}

int Data_out::Get_Id() const
{
    return 1;
}

int Data_out::Get_Time() const
{
    return 1;
}

void Data_out::Set_Id(const int id)
{

}

void Data_out::Set_Time(const int time)
{

}

void Data_out::Set_Points(std::vector<std::pair<double, double >> &point)
{

}

//Data_out Data_out::operator=(testing::internal::OnCallSpec<Data_out(void)> Reight)
//{
//    Data_out a;
//    return a;
//}

void Data_out::Get_Points(std::vector<std::pair<double, double >> &point) const {

}
