#include "SEMGibbs.h"

SEMGibbs::SEMGibbs()
{
  // TODO Auto-generated constructor stub

}

SEMGibbs::~SEMGibbs()
{
  // TODO Auto-generated destructor stub
}

void SEMGibbs::run()
{
  p_model_->seStep();
  p_model_->mStep();
}
