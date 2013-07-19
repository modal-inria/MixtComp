#include "CEMAlgo.h"

CEMAlgo::CEMAlgo()
{
  // TODO Auto-generated constructor stub

}

CEMAlgo::~CEMAlgo()
{
  // TODO Auto-generated destructor stub
}

void CEMAlgo::run(IModel*& p_model){
  p_model->ceStep();
  p_model->mStep();
}
