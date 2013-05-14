#include "CompositeDeveloper.h"
#include <iostream>

CompositeDeveloper::CompositeDeveloper() {
}

CompositeDeveloper::CompositeDeveloper(const CompositeDeveloper& other){
  for (int i = 0; i < other.v_Developer_.size(); ++i) {
    this->v_Developer_.push_back(other.v_Developer_[i]->clone());
  }
}

CompositeDeveloper::CompositeDeveloper(const std::vector<IDeveloper*>& developerlist) {
  for (int i = 0; i < developerlist.size(); ++i) {
    v_Developer_.push_back(developerlist[i]->clone());
  }
}

CompositeDeveloper::~CompositeDeveloper(){
  for (int i = 0; i < v_Developer_.size(); ++i) {
    if(v_Developer_[i]!=NULL)
    {
      delete v_Developer_[i];
      v_Developer_[i] = NULL;
    }
  }
}

CompositeDeveloper * CompositeDeveloper::clone() {
  CompositeDeveloper* temp = new CompositeDeveloper();
  for (int i = 0; i < this->v_Developer_.size(); ++i) {
    temp->v_Developer_.push_back(this->v_Developer_[i]->clone());
  }
  return temp;
}

void CompositeDeveloper::setModel(Model* model) {
  for (int i = 0; i < v_Developer_.size(); ++i) {
      v_Developer_[i]->setModel(model);
    }
}

void CompositeDeveloper::initializeStep() {
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->initializeStep();
  }
}

void CompositeDeveloper::imputationStep()
{
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->imputationStep();
  }
}

void CompositeDeveloper::samplingStep()
{
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->samplingStep();
  }
}

void CompositeDeveloper::paramUpdateStep()
{
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->paramUpdateStep();
  }
}

void CompositeDeveloper::storeIntermediateResults(int iteration){
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->storeIntermediateResults(iteration);
  }
}
void CompositeDeveloper::finalizeStep() {
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->finalizeStep();
  }
}

double CompositeDeveloper::posteriorProbability(int sample_num,int cluster_num) {
  double pdfval = 1.0;
  for (int i = 0; i < v_Developer_.size(); ++i) {
    pdfval*= v_Developer_[i]->posteriorProbability(sample_num,cluster_num);
  }
  return pdfval;
}

int CompositeDeveloper::freeParameters() const {
  int freeparam = 0;
  for (int i = 0; i < v_Developer_.size(); ++i) {
    freeparam+= v_Developer_[i]->freeParameters();
  }
  return freeparam;
}

double CompositeDeveloper::logLikelihood() {
  double val = 0;
  for (int i = 0; i < v_Developer_.size(); ++i) {
    val+= v_Developer_[i]->logLikelihood();
  }
  return val;
}

void CompositeDeveloper::setData() {
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->setData();
  }
}
void CompositeDeveloper::writeParameters(std::ostream & out) const {
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->writeParameters(out);
    out<<"*****************************************************\n\n";
  }
}
