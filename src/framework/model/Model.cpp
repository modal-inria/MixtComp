#include <iostream>
#include <math.h>
#include "framework/DeveloperInterface/IDeveloper.h"
#include "Model.h"

Model::Model(IDeveloper* developer,int nbsample,int nbcluster) : nbSample_(nbsample),
                                                                 nbCluster_(nbcluster)
{
  //Allocate memory for conditional probabilities
  m_Tik_ = new double*[nbsample];
  for (int i = 0; i < nbsample; ++i) {
    m_Tik_[i] = new double[nbcluster];
  }

  //Allocate memory for class labels
  v_Zi_  = new int[nbsample];

  //Allocate memory for row proportions
  v_Pie_ = new double[nbcluster];

  //Allocate memory for developer
  p_developer_ = developer->clone();
  //set this pointer
  p_developer_->setModel(this);
  //set data for developer
  p_developer_->setData();
}

Model::Model(const Model& other){
  nbSample_ = other.nbSample_;
  nbCluster_ = other.nbCluster_;

  //Allocate memory for conditional probabilities and copy values
  m_Tik_ = new double*[nbSample_];
  for (int i = 0; i < nbSample_; ++i) {
    m_Tik_[i] = new double[nbCluster_];
  }

  for (int i = 0; i < nbSample_; ++i) {
    for (int j = 0; j < nbCluster_; ++j) {
      m_Tik_[i][j] = other.m_Tik_[i][j];
    }
  }

  //Allocate memory for class labels and copy values
  v_Zi_  = new int[nbSample_];

  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = other.v_Zi_[i];
  }
  //Allocate memory for row proportions and copy values
  v_Pie_ = new double[nbCluster_];

  for (int j = 0; j < nbCluster_; ++j) {
    v_Pie_[j] = other.v_Pie_[j];
  }

  //Allocate memory for developer
  p_developer_ = other.p_developer_->clone();
  //set this pointer
  p_developer_->setModel(this);

}

Model* Model::clone(){
  return new Model(*this);
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

void Model::storeIntermediateResults(int iteration){
  p_developer_->storeIntermediateResults(iteration);
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
  // update t_ik
  for (int i = 0; i < nbSample_; ++i) {
    double sum = 0;
    for (int k = 0; k < nbCluster_; ++k) {
      sum+=p_developer_->posteriorProbability(i,k);
    }

    for (int k = 0; k < nbCluster_; ++k) {
      m_Tik_[i][k] = p_developer_->posteriorProbability(i,k)/sum;
    }
  }

  // update z_i

  srand(time(0));
  for (int i = 0; i < nbSample_; ++i) {
    double randval = float(std::rand())/float(RAND_MAX);
    double cumsum = m_Tik_[i][0];
    for (int k = 1; k < nbCluster_; ++k) {
      if(randval<=cumsum)
      {
        v_Zi_[i] = k-1;
        break;
      }
      cumsum+=m_Tik_[i][k];
    }
  }
  // update pie

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k] = 0.0;
  }
  for (int i = 0; i < nbSample_; ++i) {
    v_Pie_[v_Zi_[i]]+=1.0;
  }

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k]/=nbSample_;
  }

}

void Model::randomInitialization(){
  //generate random numbers
  srand(time(0));
  std::vector<float> randnumbers(nbSample_);
  for ( int i = 0; i < nbSample_; ++i) {
    randnumbers[i] = float(std::rand())/float(RAND_MAX);
  }

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k] = 0.0;
  }

  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = floor(nbCluster_*randnumbers[i]);
    v_Pie_[v_Zi_[i]]+=1.0;
  }

  for (int k = 0; k < nbCluster_; ++k) {
    v_Pie_[k]/=nbSample_;
  }
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
      m_Tik_[i][j] = other.m_Tik_[i][j];
    }
  }

  //copy values for class labels
  for (int i = 0; i < nbSample_; ++i) {
    v_Zi_[i] = other.v_Zi_[i];
  }
  //copy values for row proportions
  for (int j = 0; j < nbCluster_; ++j) {
    v_Pie_[j] = other.v_Pie_[j];
  }

  return *this;
}

void Model::writeParameters(std::ostream&out) const{
  p_developer_->writeParameters(out);
}
