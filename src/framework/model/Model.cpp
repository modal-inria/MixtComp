#include "framework/DeveloperInterface/IDeveloper.h"
#include "Model.h"

Model::Model()
{
  // TODO Auto-generated constructor stub
}

Model::Model(const Model& other){
  // TODO
}

Model::~Model()
{
  // TODO Auto-generated destructor stub
}

void Model::mStep()
{
  p_developer_->paramUpdateStep();
}

void Model::seStep()
{
  p_developer_->imputationStep();
  p_developer_->samplingStep();
  updateModelParameters();
}
void Model::initializeModel()
{
  randomInitialization();
  p_developer_->initializeStep();
}

void Model::finalizeModel()
{
  p_developer_->finalizeStep();
}

void Model::updateModelParameters()
{
  // TODO
}

void Model::randomInitialization(){
  // TODO
}
double Model::logLikelihood() const {
  return p_developer_->logLikelihood();
}
