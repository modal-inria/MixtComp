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
  p_developer_->imputationStep(m_Zik_);
  p_developer_->samplingStep(m_Zik_);
  updateLabels();
}
void Model::initializeModel()
{
  p_developer_->initializeStep();
}

void Model::finalizeModel()
{
  p_developer_->finalizeStep();
}

void Model::updateLabels()
{
  // TODO
}
