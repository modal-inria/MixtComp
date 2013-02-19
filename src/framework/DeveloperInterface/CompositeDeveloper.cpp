#include "CompositeDeveloper.h"

CompositeDeveloper::CompositeDeveloper() {
  // TODO Auto-generated constructor stub
}

CompositeDeveloper::CompositeDeveloper(std::vector<IDeveloper*> developerlist) {
  v_Developer_ = developerlist;
}

CompositeDeveloper::~CompositeDeveloper() {
  // TODO Auto-generated constructor stub
}

CompositeDeveloper * CompositeDeveloper::clone() const {
  return new CompositeDeveloper(this);
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

void CompositeDeveloper::finalizeStep() {
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->finalizeStep();
  }
}

double CompositeDeveloper::posteriorProbability(int sample_num,int cluster_num) const {
  double pdfval = 1.0;
  for (i = 0; i < v_Developer_.size(); ++i) {
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

double CompositeDeveloper::logLikelihood() const {
  double val = 0;
  for (int i = 0; i < v_Developer_.size(); ++i) {
    val+= v_Developer_[i]->logLikelihood();
  }
  return freeparam;
}

void CompositeDeveloper::writeParameters(std::ostream & out) {
  for (int i = 0; i < v_Developer_.size(); ++i) {
    v_Developer_[i]->writeParameters(out);
    out<<"*****************************************************\n\n";
  }
}
