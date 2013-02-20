#include "SEMGibbs.h"

SEMGibbs::SEMGibbs()
{
  // TODO Auto-generated constructor stub

}

SEMGibbs::~SEMGibbs()
{
  // TODO Auto-generated destructor stub
}

void SEMGibbs::run(Model* p_model)
{
  p_model->seStep();
  p_model->mStep();
}
