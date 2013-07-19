#include "EMAlgo.h"

EMAlgo::EMAlgo()
{
  // TODO Auto-generated constructor stub

}

EMAlgo::~EMAlgo()
{
  // TODO Auto-generated destructor stub
}


void EMAlgo::run(IModel*& p_model){
  p_model->eStep();
  p_model->mStep();
}
