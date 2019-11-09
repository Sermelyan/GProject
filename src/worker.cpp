#include "worker.hpp"

//Worker::Worker()
//{
//
//}

//Worker::Worker(std::queue<Data_in> &in, std::queue<Data_out> &out);
//{
//
//}

Worker::~Worker()
{

}

Data_in Worker::GetFromQueu()
{
    Data_in a;
    return a;
}

void  Worker::SendToQueu(const Data_out value)
{

}
void Worker::GetDotsFromDB(std::vector<std::pair<double, double>> &points, Data_in value)
{

}

void Worker::GetRibsFromAPI(std::vector<std::pair<double, double>> &points, std::vector<std::vector<double >> &ribs)
{

}

void Worker::GetRoute(std::vector<std::pair<double, double>> &points, std::vector<std::vector<int>> &ribs, std::list<std::pair<double, double>> res)
{

}

void Worker::Check()
{

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

bool operator==(const Data_out left, const Data_out right){
    return true;
}

Data_out::Data_out()
{

}

Data_out::Data_out(const std::vector<std::pair<double, double >> &point, const int id, const int time)
{
    this->Points.clear();
    for(int i = 0; i < point.size(); ++i){
        this->Points.push_back(point[i]);
    }
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

Data_out Data_out::operator=(const Data_out &Reight)
{
    Data_out a;
    return a;
}

void Data_out::Get_Points(std::vector<std::pair<double, double >> &point) const {

}
