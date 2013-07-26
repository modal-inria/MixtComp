#include "IModel.h"

IModel::IModel()
{
  // TODO Auto-generated constructor stub

}

IModel::~IModel()
{
  // TODO Auto-generated destructor stub
}

IModel& IModel::operator=(const IModel& other){
  baseparameters_=other.baseparameters_;
}
