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

void Model::Estep()
{
  p_MixtureLaw_->Estep();
}

void Model::Initialize()
{
  p_MixtureLaw_->InitializeStep();
}

void Model::Finalize()
{
  p_MixtureLaw_->FinalizeStep();
}
