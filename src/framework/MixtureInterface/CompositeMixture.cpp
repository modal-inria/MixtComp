#include "CompositeMixture.h"
#include <iostream>

CompositeMixture::CompositeMixture() {
}

CompositeMixture::CompositeMixture(const CompositeMixture& other){
  for (int i = 0; i < other.v_Mixture_.size(); ++i) {
    this->v_Mixture_.push_back(other.v_Mixture_[i]->clone());
  }
}

CompositeMixture::CompositeMixture(const std::vector<IMixture*>& Mixturelist) {
  for (int i = 0; i < Mixturelist.size(); ++i) {
    v_Mixture_.push_back(Mixturelist[i]->clone());
  }
}

CompositeMixture::~CompositeMixture(){
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    if(v_Mixture_[i]!=NULL)
    {
      delete v_Mixture_[i];
      v_Mixture_[i] = NULL;
    }
  }
}

CompositeMixture * CompositeMixture::clone() {
  CompositeMixture* temp = new CompositeMixture();
  for (int i = 0; i < this->v_Mixture_.size(); ++i) {
    temp->v_Mixture_.push_back(this->v_Mixture_[i]->clone());
  }
  return temp;
}

void CompositeMixture::setModel(Model* model) {
  for (int i = 0; i < v_Mixture_.size(); ++i) {
      v_Mixture_[i]->setModel(model);
    }
}

void CompositeMixture::initializeStep() {
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->initializeStep();
  }
}

void CompositeMixture::imputationStep()
{
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->imputationStep();
  }
}

void CompositeMixture::samplingStep()
{
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->samplingStep();
  }
}

void CompositeMixture::paramUpdateStep()
{
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->paramUpdateStep();
  }
}

void CompositeMixture::storeIntermediateResults(int iteration){
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->storeIntermediateResults(iteration);
  }
}
void CompositeMixture::finalizeStep() {
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->finalizeStep();
  }
}

double CompositeMixture::posteriorProbability(int sample_num,int cluster_num) {
  double pdfval = 1.0;
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    pdfval*= v_Mixture_[i]->posteriorProbability(sample_num,cluster_num);
  }
  return pdfval;
}

int CompositeMixture::freeParameters() const {
  int freeparam = 0;
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    freeparam+= v_Mixture_[i]->freeParameters();
  }
  return freeparam;
}

double CompositeMixture::logLikelihood() {
  double val = 0;
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    val+= v_Mixture_[i]->logLikelihood();
  }
  return val;
}

void CompositeMixture::setData() {
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->setData();
  }
}
void CompositeMixture::writeParameters(std::ostream & out) const {
  for (int i = 0; i < v_Mixture_.size(); ++i) {
    v_Mixture_[i]->writeParameters(out);
    out<<"*****************************************************\n\n";
  }
}
