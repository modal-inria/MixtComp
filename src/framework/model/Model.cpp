#include "Model.h"

Model::Model()
{
  // TODO Auto-generated constructor stub

}

Model::~Model()
{
  // TODO Auto-generated destructor stub
}

void Model::mStep()
{
  p_developer_->paramUpdateStep(m_Zik);
}

void Model::seStep()
{
  p_developer_->imputationStep(v_Pie);
  p_developer_->samplingStep(v_Pie,m_Zik);
  updateLabels();
}
void Model::initializeModel()
{
  p_developer_->initializeStep();
}

void Model::finalize()
{
  p_developer_->finalizeStep();
}

void Model::updateLabels()
{
  // TODO
}
