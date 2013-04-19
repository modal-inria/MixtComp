#include "framework/DeveloperInterface/IDeveloper.h"
#include "Model.h"

Model::Model(IDeveloper* developer,int nbsample,int nbcluster) : nbSample_(nbsample),nbCluster_(nbcluster)
{
  //Allocate memory for developer
  p_developer_ = developer->clone();

  //Allocate memory for conditional probabilities
  m_Tik_ = new double*[nbsample];
  for (int i = 0; i < nbsample; ++i) {
    m_Tik_[i] = new double[nbcluster];
  }

  //Allocate memory for class labels
  v_Zi_  = new int[nbsample];

  //Allocate memory for row proportions
  v_Pie_ = new double[nbcluster];

}

Model::Model(const Model* other){
  nbSample_ = other->nbSample_;
  nbCluster_ = other->nbCluster_;

  //Allocate memory for developer
  p_developer_ = other->developer->clone();

  //Allocate memory for conditional probabilities and copy values
  m_Tik_ = new double*[nbsample];
  for (int i = 0; i < nbsample; ++i) {
    m_Tik_[i] = new double[nbcluster];
  }

  for (int i = 0; i < nbSample_; ++i) {
    for (int j = 0; j < nbCluster_; ++j) {
      m_Tik_[i][j] = other->m_Tik_[i][j];
    }
  }

  //Allocate memory for class labels and copy values
  v_Zi_  = new int[nbsample];

  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = other->v_Zi_[i];
  }
  //Allocate memory for row proportions and copy values
  v_Pie_ = new double[nbcluster];

  for (int j = 0; j < nbCluster_; ++j) {
    v_Pie_[j] = other->v_Pie_[j];
  }

}

Model::~Model()
{
  //release various memories
  delete p_developer_;
  delete[] v_Pie_;
  delete[] v_Zi_;
  for (int i = 0; i < nbSample_; ++i) {
    delete[] m_Tik_[i];
  }
  delete[] m_Tik_;
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

Model& Model::operator=(const Model& other){
  //copy developer
  *p_developer_ = *other.p_developer_;

  //copy values for conditional probabilities
  for (int i = 0; i < nbSample_; ++i) {
    for (int j = 0; j < nbCluster_; ++j) {
      m_Tik_[i][j] = other->m_Tik_[i][j];
    }
  }

  //copy values for class labels
  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = other->v_Zi_[i];
  }
  //copy values for row proportions
  for (int j = 0; j < nbCluster_; ++j) {
    v_Pie_[j] = other->v_Pie_[j];
  }

  return *this;
}
