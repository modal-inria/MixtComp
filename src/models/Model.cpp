#include "Model.h"

Model::Model()
{
  // TODO Auto-generated constructor stub

}

Model::~Model()
{
  // TODO Auto-generated destructor stub
}

void Model::Mstep()
{
  p_MixtureLaw_->Mstep();
}
