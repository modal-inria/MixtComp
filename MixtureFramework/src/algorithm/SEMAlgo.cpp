#include "SEMAlgo.h"

SEMAlgo::SEMAlgo()
{
  // TODO Auto-generated constructor stub

}

SEMAlgo::~SEMAlgo()
{
  // TODO Auto-generated destructor stub
}

void SEMAlgo::run(IModel*& p_model){
  p_model->seStep();
  p_model->mStep();
}
